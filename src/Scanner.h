#pragma once

#include "Token.h"
#include <istream>
#include <stdexcept>
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
public:
	explicit Scanner(std::istream& code);

	std::vector<Token> scan();

private:
	std::istream& code;
	unsigned int line{};

	int scanInt();
	bool skipWhitespace();
	bool isWhitespace(char c);
};
