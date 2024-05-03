#include "scanner.h"
#include "token.h"
#include <cassert>
#include <cctype>
#include <cstdlib>
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
	case ')': return getToken(TokType::close_paren);
	case ';': return getToken(TokType::Semi);
	case '=': return getToken(TokType::Equal);
	case '{': return getToken(TokType::OpenBrace);
	case '}': return getToken(TokType::OpenBrace);
	}

	if (isAlpha(c)) return scanIdent();
	if (isDigit(c)) return scanNumber();

	return getToken(TokType::Error);
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
	while (not isEmpty() && isDigit(peek()))
		next();

	return getToken(TokType::number);
}

Token Scanner::scanIdent()
{
	// extremely inefficient, will optimize when compiler grows
	while (not isEmpty() && (isAlpha(peek()) || isDigit(peek())))
		next();

	auto text = std::string(sourceCode.substr(start, current));
	if (keywords.contains(text))
		return getToken(keywords.at(text));

	return getToken(TokType::Indent);
}

bool Scanner::isAlpha(char c) const
{
	return std::isalpha(static_cast<unsigned char>(c)) || '_' == c;
}

bool Scanner::isDigit(char c) const
{
	return std::isdigit(static_cast<unsigned char>(c));
}
