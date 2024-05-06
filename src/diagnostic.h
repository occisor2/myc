#pragma once

#include "token.h"
#include <stdexcept>
#include <string>
#include <string_view>

class Diagnostic : public std::runtime_error
{
public:
	enum class Type
	{
		Error,
		Warning
	};

	Diagnostic(Type type, Token badToken, std::string fileName, std::string msg);

	void print(std::ostream& out, std::string_view sourceCode) const;

private:
	// Type of diagnostic
	Type type;
	// token that caused the error or beginning location of error
	Token badToken;
	// file where error occured
	std::string fileName;
	// error message
	std::string msg;

	void printLine(std::ostream& os, std::string_view sourceCode) const;
};
