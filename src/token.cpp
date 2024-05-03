#include "token.h"
#include <cstddef>
#include <memory>

Token::Token(Type type, size_t pos, size_t size, int line)
	: type(type), pos(pos), size(size), line(line)
{}

Token::Token(const Token& t)
	: type(t.type), pos(t.pos), size(t.size), line(t.line)
{}

Token& Token::operator=(Token const& t)
{
	if (this != &t)
	{
		std::destroy_at(this);
		std::construct_at(this, t);
	}
	return *this;
}

bool Token::isNumber() const
{
	return Type::Number == type;
}

bool Token::isEof() const
{
	return Type::Eof == type;
}
