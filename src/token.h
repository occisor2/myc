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
		// don't change order above this
		close_paren,
		Semi,
		Equal,
		OpenBrace,
		CloseBrace,
		Indent,
		Error,
		// keywords
		Int
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
