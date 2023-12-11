#include "Scanner.h"
#include "Token.h"
#include <array>
#include <charconv>
#include <ios>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>


ScanError::ScanError(const char* message, unsigned int line) noexcept
	: std::runtime_error(message), message(message), line(line)
{}

const char* ScanError::what() const noexcept
{
	return std::runtime_error::what();
}

Scanner::Scanner(std::istream& code)
	: code(code)
{}

Token Scanner::scan()
{
	// If skipWhitespace returns false, then an EOF token will be
	// returned.
	current = Token(Type::Eof);

	// skipWhitespace will return false if it runs out of characters.
	if (skipWhitespace())
	{	
		Token t;
		char c = code.get();
		
		switch (c)
		{
		case '+':
			t.type = Type::Plus;
			break;
		case '-':
			t.type = Type::Minus;
			break;
		case '*':
			t.type = Type::Star;
			break;
		case '/':
			t.type = Type::Slash;
			break;
		default:
			if (std::isdigit(c))
			{
				// This is the first char of the number string, so it
				// needs to be in the stream for scanning.
				code.putback(c);
				t.type = Type::IntLit;
				t.intLit = scanInt();
				break;
			}
			
			throw ScanError("unrecognized token", line);
		}

		current = t;
	}

	return current;
}

Token Scanner::peek()
{
	return current;
}

unsigned int Scanner::getLine()
{
	return line;
}

int Scanner::scanInt()
{
	std::string num;
	int result{};
	char c{};
	
	for(;;)
	{
		c = code.get();
		// Check if peek() failed because of EOF
		if (code.eof())
			break;

		if (std::isdigit(c))
			num.push_back(c);
		else
		{
			code.putback(c);
			break;
		}
	}
	
	// Convert string to a number
	std::from_chars(num.data(), num.data() + num.size(), result);

	return result;
}

std::string Scanner::scanIdent()
{
	//char c = code.peek();

	return "";
}

bool Scanner::skipWhitespace()
{
	for (;;)
	{
		char c = code.peek();
		// Check if peek() failed because of EOF
		if (code.eof())
			return false;
		
		if (isWhitespace(c))
		{
			code.ignore(1, c);
		}
		else
			return true;
	}
}

bool Scanner::isWhitespace(char c)
{
	switch (c)
	{
	case '\n':
		line++;
		[[fallthrough]];
	case ' ':
	case '\t':
	case '\r':
	case '\f':
		return true;
	}

	return false;
}
