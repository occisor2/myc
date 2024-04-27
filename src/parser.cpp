#include "parser.h"
#include "ast/expr.h"
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

using TokType = Token::Type;

Parser::Parser(std::string_view sourceCode)
	: sourceCode(sourceCode), scanner(sourceCode),
	  prev(Token::Type::eof, 0, 0 ,0),
	  current(Token::Type::eof, 0, 0 ,0)
{}

AST Parser::parse()
{
	
}

void Parser::error(std::string_view msg)
{
	assert(((void)msg, false));
}

std::string Parser::getTokenText(Token token)
{
	return std::string(sourceCode.substr(token.pos, token.size));
}

void Parser::next()
{
	prev = current;
	current = scanner.scan();
}

void Parser::consume(Token::Type type, std::string_view errmsg)
{
	if (current.type == type)
		next();
	else
		error(errmsg);
}

Parser::RuleEntry Parser::getRuleEntry(Token::Type type)
{
	return table[static_cast<size_t>(type)];
}

std::unique_ptr<Expr> Parser::parseExpression(Precedence prec)
{
	next();
	auto prefix = getRuleEntry(prev.type).prefix;
	if (not prefix)
		assert(((void)"no prefix function found", false));

	auto left = std::invoke(prefix, this);

	while (prec <= getRuleEntry(current.type).prec)
	{
		next();
		auto infix = getRuleEntry(prev.type).infix;
		std::invoke(infix, this);
	}
}
