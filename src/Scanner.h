#pragma once

#include "Token.h"
#include <cstddef>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

class ScanError : std::runtime_error
{
public:
	ScanError(const char* message, unsigned int line) noexcept;
	const char* what() const noexcept override;

	std::string message;
	unsigned int line;
};

class Scanner
{
	using Type = Token::Type;
	static constexpr auto maxIdentLen = 32;
	
public:
	Scanner(std::istream& code = std::cin);

	//std::vector<Token> scan();

	/**
	 * Scans the next token
	 */
	Token scan();
	Token peek();
	/**
	 * Gets the line the current token 
	 */
	unsigned int getLine();

	/**
	 * Peeks at the current token.
	 */
	//Token peek();
	/**
	 * Skips the current token.
	 */
	//void skip();

private:
	std::istream& code;
	Token current = Token(Type::Eof);
	unsigned int line = 1;

	int scanInt();
	std::string scanIdent();
	bool skipWhitespace();
	bool isWhitespace(char c);
};
