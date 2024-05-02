#include "scanner.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include <string_view>

using TokType = Token::Type;

Scanner::Scanner(std::string_view sourceCode)
	: sourceCode(sourceCode), start(0), current(0), line(1)
{}

Token Scanner::scan()
{	
	skipWhitespace();
	start = current;
	if (isEmpty())
		return getToken(TokType::eof);
	
	char c = next();

	switch (c)
	{
	case '-': return getToken(TokType::minus);
	case '+': return getToken(TokType::plus);
	case '/': return getToken(TokType::slash);
	case '*': return getToken(TokType::star);
	case '(': return getToken(TokType::open_paren);
	case ')':  return getToken(TokType::close_paren);
	}
	
	return scanNumber();
}

bool Scanner::isEmpty() const
{
	return current >= sourceCode.size();
}

char Scanner::next()
{
	assert(((void)("read past the end of token stream"), not isEmpty()));
	return sourceCode[current++];
}

char Scanner::peek() const
{
	assert(((void)("read past the end of token stream"), not isEmpty()));
	return sourceCode[current];
}

void Scanner::skipWhitespace()
{
	for (;;)
	{
		if (isEmpty())
			return;
		
		char c = peek();
		switch (c)
		{
		case '\n':
			line++;
			[[fallthrough]];
		case ' ':
		case '\r':
		case '\t':
			next();
			break;
		default:
			return;
		}
	}
}

Token Scanner::getToken(Token::Type type)
{
	return Token(type, start, current - start, line);
}

Token Scanner::scanNumber()
{
	while (not isEmpty() && std::isdigit(peek()))
		next();

	return getToken(TokType::number);
}
