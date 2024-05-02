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

using TokType = Token::Type;

Parser::Parser(std::string_view sourceCode)
	: sourceCode(sourceCode), scanner(sourceCode),
	  prev(Token::Type::eof, 0, 0 ,0),
	  current(Token::Type::eof, 0, 0 ,0)
{}

void Parser::parse()
{
	next();
	auto exp = parseExpression();
	PrintAST p;
	p.visit(exp);
	std::cout << std::endl;
}

void Parser::error(std::string_view msg)
{
	std::cerr << msg << std::endl;
	assert(false);
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

void Parser::consume(Token::Type type, std::string_view errmsg)
{
	if (current.type == type)
		next();
	else
		error(errmsg);
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
		error("expected expression");

	auto left = std::invoke(rule.prefix, this);

	while (getRule(current.type).prec >= precedence)
	{
		next();
		auto infix = getRule(prev.type).infix;
		if (!infix)
			error("exptected infix operator");

		left = std::invoke(infix, this, std::move(left));
	}

	return left;
}

std::unique_ptr<Expr> Parser::primary()
{
	int result{};
	auto text = sourceCode.substr(prev.pos, prev.size);
	auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), result);

	assert(((void)"not a number", std::errc::invalid_argument != ec));

	if (std::errc::result_out_of_range ==  ec)
		throw std::runtime_error("number is out of range");

	return std::make_unique<Expr>(NumLit(result));
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
	case Token::Type::minus:
		opType = OpType::Sub;
		break;
	case Token::Type::plus:
		opType = OpType::Add;
		break;
	case Token::Type::slash:
		opType = OpType::Div;
		break;
	case Token::Type::star:
		opType = OpType::Mul;
		break;
	default: assert(((void)"not an infix operator", false));
	}

	return std::make_unique<Expr>(BinExp(opType, std::move(left), std::move(right)));
}
