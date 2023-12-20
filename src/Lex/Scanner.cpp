#include "Scanner.h"
#include "Token.h"
#include "error.h"
#include <cctype>
#include <charconv>
#include <format>
#include <istream>
#include <string>

Scanner::Scanner(std::istream& code, std::string fileName)
	: code(code), fileName(fileName)
{}

Token Scanner::scan()
{
	auto nextToken = getNext();
	current = nextToken;
	return current;
}

Token Scanner::next()
{
	auto t = current;
	scan();
	return t;
}

Token Scanner::getNext()
{
	// Check if a token was putback
	if (wasPutback)
	{
		wasPutback = false;
		return current;
	}
	
	// skipWhitespace will return false if it runs out of characters.
	if (skipWhitespace())
	{	
		char c = code.get();
		char nextChar;
		
		switch (c)
		{
		case '+':
			return Token(Type::Plus, std::string(1, c));
		case '-':
			return Token(Type::Minus, std::string(1, c));
		case '*':
			return Token(Type::Star, std::string(1, c));
		case '/':
			return Token(Type::Slash, std::string(1, c));
		case ';':
			return Token(Type::Semi, std::string(1, c));
		case '{':
			return Token(Type::OpenBrace, std::string(1, c));
		case '}':
			return Token(Type::CloseBrace, std::string(1, c));
		case '(':
			return Token(Type::OpenParen, std::string(1, c));
		case ')':
			return Token(Type::CloseParen, std::string(1, c));
		case '=':
			nextChar = code.get();
			if ('=' == nextChar)
				return Token(Type::Equal, "==");
			code.putback(nextChar);
			return Token(Type::Assign, std::string(1, c));
		case '!':
			nextChar = code.get();
			if ('=' == nextChar)
				return Token(Type::NotEqual, "!=");
			code.putback(nextChar);
			return Token(Type::Not, std::string(1, c));	
		case '>':
			nextChar = code.get();
			if ('=' == nextChar)
				return Token(Type::GreaterEqual, ">=");
			code.putback(nextChar);
			return Token(Type::Greater, std::string(1, c));
		case '<':
			nextChar = code.get();
			if ('=' == nextChar)
				return Token(Type::LessEqual, "<=");
			code.putback(nextChar);
			return Token(Type::Less, std::string(1, c));
		default:
			if (std::isalpha(static_cast<unsigned char>(c)) || '_' == c)
			{
				code.putback(c);
				return scanIdent();
			}
			else if (std::isdigit(static_cast<unsigned char>(c)))
			{
				// This is the first char of the number string, so it
				// needs to be in the stream for scanning.
				code.putback(c);
				return scanInt();
			}
		   
			panic(std::format("unrecognized token at character '{}'", c));
		}
	}

	return Token(Type::Eof, "EOF");
}

Token Scanner::peek() const
{
	// Check if a token was putback
	if (wasPutback)
		return previous;
	return current;
}

void Scanner::putback(Token t)
{
	previous = t;
	wasPutback = true;
}

bool Scanner::eof() const
{
	return peek().getType() == Type::Eof;
}

unsigned int Scanner::getLine() const
{
	return line;
}

std::string Scanner::getFileName() const
{
	return fileName;
}

void Scanner::panic(std::string message)
{
	error::fatal(message, fileName, line);
}

Token Scanner::scanInt()
{
	std::string numberStr;
	int result{};
	char c{};
	
	for(;;)
	{
		c = code.get();
		if (code.eof())
			break;

		if (std::isdigit(static_cast<unsigned char>(c)))
			numberStr.push_back(c);
		else
		{
			code.putback(c);
			break;
		}
	}

	if (std::isalpha(static_cast<unsigned char>(code.peek())) || '_' == c)
		panic(std::format("invalid digit '{}' in integer", code.peek()));
	
	// Convert string to a number
	std::from_chars(numberStr.data(), numberStr.data() + numberStr.size(), result);

	return Token(result, numberStr);
}

Token Scanner::scanIdent()
{
	std::string ident;
	auto i = 0;

	for(;;)
	{
		if (i >= maxIdentLen)
			panic(std::format("reached the max identifier length in identifier '{}'", ident));
		
		char c = code.get();
		if (code.eof())
			break;

		if (std::isalnum(static_cast<unsigned char>(c)) || '_' == c)
			ident.push_back(c);
		else
		{
			code.putback(c);
			break;
		}

		++i;
	}

	if (keywords.contains(ident))
		return Token(keywords.at(ident), ident);

	return Token(ident);
}

bool Scanner::skipWhitespace()
{
	for (;;)
	{
		auto c = code.peek();
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
