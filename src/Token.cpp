#include "Token.h"
#include <string>

using Type = Token::Type;

Token::Token(Type type)
	: type(type)
{}

Token::Token(Type type, int intLit)
	: type(type), intLit(intLit)
{}

std::ostream& operator<<(std::ostream& out, const Token& t)
{
	std::string typeStr;

	switch (t.type)
	{
	case Token::Type::Plus:
		typeStr = "+";
		break;
	case Token::Type::Minus:
		typeStr = "-";
		break;
	case Token::Type::Star:
		typeStr = "*";
		break;
	case Token::Type::Slash:
		typeStr = "/";
		break;
	case Token::Type::IntLit:
		typeStr = std::to_string(t.intLit);
		break;
	}

	return out << typeStr;
}
