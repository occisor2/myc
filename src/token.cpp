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
	return Type::number == type;
}

bool Token::isEof() const
{
	return Type::eof == type;
}

// int Token::toNumber()
// {
// 	assert(((void)("token type is not a number"), type == Type::number));
	
// 	int result{};
// 	auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);

// 	// Scanner should correctly parse what a number is
// 	assert(((void)("invalid argument to toNumber"), std::errc::invalid_argument != ec));
	
// 	if (std::errc::result_out_of_range == ec)
// 		throw std::runtime_error("number to big to fit in integer");

// 	return result;
// }
