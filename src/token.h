#pragma once

#include <cstddef>

class Token
{
public:
	enum class Type : size_t
	{
		eof,
		number,
		minus,
		plus,
		slash,
		star,
		open_paren,
		close_paren,
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
