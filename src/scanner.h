#pragma once

#include "token.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>

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
	Token scanIdent();
	bool isAlpha(char c) const;
	bool isDigit(char c) const;

	// will be replaced by a trie eventually
	std::unordered_map<std::string, Token::Type> keywords = {
		{"int", Token::Type::Int}
	};
};
