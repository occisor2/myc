#pragma once

#include <cstddef>

class Token
{
public:
	enum class Type : size_t
	{
		open_paren,
		minus,
		plus,
		slash,
		star,
		number, // at and above this line, members need ordering for parser
		close_paren,
		eof
	};

	const Type type;
	const size_t pos;
	const size_t size;
	const int line;

	Token() = delete;
	Token(Type type, size_t pos, size_t size, int line);
	Token(const Token& t);
	Token& operator=(Token const& t);

	bool isNumber() const;
	bool isEof() const;
};
