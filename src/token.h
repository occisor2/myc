#pragma once

#include <cstddef>

class Token
{
public:
	// don't change order, needed by parser
	enum class Type : size_t
	{
		Eof,
		Number,
		Minus,
		Plus,
		Slash,
		Star,
		OpenParen,
		CloseParen,
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
