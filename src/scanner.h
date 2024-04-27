#pragma once

#include "token.h"
#include <cstddef>
#include <string_view>

class Scanner
{
public:
	Scanner() = delete;
	Scanner(std::string_view sourceCode);

	Token scan();
	bool isEmpty() const;

private:
	const std::string_view sourceCode;
	size_t start;
	size_t current;
	int line;

	char next();
	char peek() const;
	void skipWhitespace();
	Token getToken(Token::Type type);
	Token scanNumber();
};
