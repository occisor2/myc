#include "parser.h"
#include "analysis.h"
#include "ast.h"
#include "token.h"
#include <cassert>
#include <cstddef>
#include <charconv>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

using TokType = Token::Type;

Parser::Parser(std::string_view sourceCode, std::string fileName)
	: fileName(fileName), sourceCode(sourceCode), scanner(sourceCode),
	  prev(Token::Type::Eof, 0, 0 ,0),
	  current(Token::Type::Eof, 0, 0 ,0)
{}

void Parser::parse()
{
	PrintAST p;
	next();
	try
	{
		auto func = parseFunction();
		p.visit(*func);
	}
	catch (SyntaxError& e)
	{
		// print the error message and quit parsing
		e.print(std::cout, sourceCode);
	}
}

void Parser::error(const std::string& msg, Token badToken)
{
	auto newMsg = std::string("syntax error: ") + msg;
	throw SyntaxError(badToken, fileName, newMsg);
}

std::string Parser::getTokenText(Token token)
{
	return std::string(sourceCode.substr(token.pos, token.size));
}

Token Parser::next()
{
	prev = current;
	current = scanner.scan();
	return current;
}

void Parser::consume(Token::Type type, const std::string& errmsg)
{
	if (current.type == type)
		next();
	else
		error(errmsg, current);
}

std::unique_ptr<FuncDecl> Parser::parseFunction()
{
	consume(TokType::Int, "expected function return type");

	auto nameToken = current;
	auto name = parseIdent();
	next();

	consume(TokType::OpenParen, "expected opening parenthesis");
	consume(TokType::CloseParen, "expected opening parenthesis");

	auto body = parseCompound();

	return std::make_unique<FuncDecl>(std::move(name), std::move(body), nameToken);
}

std::unique_ptr<Compound> Parser::parseCompound()
{
	auto startToken = current;
	consume(TokType::OpenBrace, "exptected opening brace");

	std::vector<std::unique_ptr<State>> block;
	while (TokType::CloseBrace != current.type)
		block.push_back(parseStatement());

	consume(TokType::CloseBrace, "unbalanced braces");

	return std::make_unique<Compound>(std::move(block), startToken);
}

std::unique_ptr<State> Parser::parseStatement()
{
	switch (current.type)
	{
	case TokType::Int:
		return parseDecl();
	case TokType::Return:
		return parseReturn();
	case TokType::OpenBrace:
		return parseCompound();
	default:
		auto expr = parseExpression();
		consume(TokType::Semi, "exptected semicolon");
		return expr;
	}
}

std::unique_ptr<State> Parser::parseReturn()
{
	auto returnTok = current;
	next();
	auto value = parseExpression();
	consume(TokType::Semi, "expected semicolon");
	return std::make_unique<ReturnState>(std::move(value), returnTok);
}

std::unique_ptr<Ident> Parser::parseIdent()
{
	if (TokType::Indent != current.type)
		error("expected proper identifier", current);
	auto ident = getTokenText(current);
	return std::make_unique<Ident>(ident, current);
}

std::unique_ptr<Decl> Parser::parseDecl()
{
	next();
	auto startToken = current;
	auto ident = parseIdent();
	next();
	consume(TokType::Equal, "expected =");
	auto right = parseExpression();
	consume(TokType::Semi, "expected semicolon");
	return std::make_unique<Decl>(std::move(ident), std::move(right), startToken);
}

Parser::Rule Parser::getRule(Token::Type type) const
{
	return table[static_cast<size_t>(type)];
}

std::unique_ptr<Expr> Parser::parseExpression()
{
	return parsePrecedence(Precedence::Assignment);
}

std::unique_ptr<Expr> Parser::parsePrecedence(Precedence precedence)
{
	next();
	auto rule = getRule(prev.type);
	if (!rule.prefix)
		error("expected expression", prev);

	auto left = std::invoke(rule.prefix, this);

	while (getRule(current.type).prec >= precedence)
	{
		next();
		auto infix = getRule(prev.type).infix;
		if (!infix)
			error("exptected binary operator", prev);

		left = std::invoke(infix, this, std::move(left));
	}

	return left;
}

std::unique_ptr<Expr> Parser::number()
{
	int result{};
	auto text = sourceCode.substr(prev.pos, prev.size);
	auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), result);

	assert(((void)"not a number", std::errc::invalid_argument != ec));

	if (std::errc::result_out_of_range ==  ec)
		throw std::runtime_error("number is out of range");

	return std::make_unique<NumLit>(result, prev);
}

std::unique_ptr<Expr> Parser::identifier()
{
	auto name = sourceCode.substr(prev.pos, prev.size);
	return std::make_unique<Ident>(std::string(name), current);
}

std::unique_ptr<Expr> Parser::grouping()
{
	auto expr = parseExpression();
	consume(TokType::CloseParen, "unbalanced parentheses");
	return expr;
}

std::unique_ptr<Expr> Parser::binary(std::unique_ptr<Expr> left)
{
	using OpType = BinExp::OpType;

	auto op = prev;
	auto rule = getRule(op.type);
	auto prec = static_cast<Precedence>(static_cast<int>(rule.prec) + 1);
	auto right = parsePrecedence(prec);

	OpType opType{};
	switch (op.type)
	{
	case Token::Type::Minus:
		opType = OpType::Sub;
		break;
	case Token::Type::Plus:
		opType = OpType::Add;
		break;
	case Token::Type::Slash:
		opType = OpType::Div;
		break;
	case Token::Type::Star:
		opType = OpType::Mul;
		break;
	default:
		assert(((void)"not a binary operator", false));
	}

	return std::make_unique<BinExp>(opType,std::move(left), std::move(right), op);
}
