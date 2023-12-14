#include "Token.h"
#include <string>

using Type = Token::Type;

Token::Token(Type type)
	: type(type)
{}

Token::Token(Type type, std::string ident)
	: type(type), ident(ident)
{}

Token::Token(std::string ident)
	: type(Type::Ident), ident(ident)
{}


Token::Token(int intLit, std::string numStr)
	: type(Type::IntLit), intLit(intLit), ident(numStr)
{}

Type Token::getType() const
{
	return type;
}

int Token::getIntLit() const
{
	return intLit;
}

std::string Token::getText() const
{
	return ident;
}

std::string Token::getIdent() const
{
	return ident;
}

std::ostream& operator<<(std::ostream& out, const Token& t)
{
	switch (t.type)
	{
	case Type::Eof:
		out << "EOF";
		break;
	default:
		out << t.ident;
	}

	return out;
}
