#pragma once

#include "Token.h"
#include <cstddef>
#include <istream>
#include <string>
#include <unordered_map>

class Scanner
{
	using Type = Token::Type;
	static constexpr auto maxIdentLen = 32;

	const std::unordered_map<std::string, Token::Type> keywords = {
		{"int", Token::Type::Int}
	};
	
public:
	Scanner(std::istream& code, std::string fileName);

	/**
	 * Scans the next token
	 */
	Token scan();
	Token peek() const;
	/**
	 * Gets the line the current token 
	 */
	unsigned int getLine() const;
	std::string getFileName() const;

private:
	std::istream& code;
	std::string fileName;
	Token current = Token(Type::Eof);
	unsigned int line = 1;

	void panic(std::string message);
	Token getNext();
	Token scanInt();
	Token scanIdent();
	bool skipWhitespace();
	bool isWhitespace(char c);
};
