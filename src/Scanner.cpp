#include "Scanner.h"
#include <charconv>
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
	: code(code), line(0)
{}

std::vector<Token> Scanner::scan()
{
	using Type = Token::Type;
	std::vector<Token> tokens;
	
	while (skipWhitespace())
	{	
		Token tok;	
		char c = code.get();
		
		switch (c)
		{
		case '+':
			tok.type = Type::Plus;
			break;
		case '-':
			tok.type = Type::Minus;
			break;
		case '*':
			tok.type = Type::Star;
			break;
		case '/':
			tok.type = Type::Slash;
			break;
		default:
			if (std::isdigit(c))
			{
				// This is the first char of the number string, so it
				// needs to be in the stream for scanning.
				code.putback(c);
				tok.type = Type::IntLit;
				tok.intLit = scanInt();
				break;
			}
			
			throw ScanError("unrecognized token", line);
		}

		tokens.push_back(tok);
	}

	return tokens;
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

bool Scanner::skipWhitespace()
{
	for (;;)
	{
		char c = code.peek();
		// Check if peek() failed because of EOF
		if (code.eof())
			return false;
		
		if (isWhitespace(c))
			code.ignore(1, c);
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
