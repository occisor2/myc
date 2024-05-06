#include "diagnostic.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>

Diagnostic::Diagnostic(Type type, Token badToken, std::string fileName, std::string msg)
	: std::runtime_error(msg), type(type), badToken(badToken),
	  fileName(fileName), msg(msg)
{}

void Diagnostic::print(std::ostream& out, [[maybe_unused]] std::string_view sourceCode) const
{
	out << fileName << ":" << badToken.line << ":" << " ";
	switch (type)
	{
	case Type::Error:
		out << "error: ";
		break;
	case Type::Warning:
		out << "warning: ";
		break;
	}
	out << msg << std::endl;

	// show location of the error
	//printLine(out, sourceCode);
}

void Diagnostic::printLine(std::ostream& out, std::string_view sourceCode) const
{
	auto startPos = badToken.pos;
	// check if token is EOF beacuse we'll have to move the position past the newline.
	size_t lineStart = badToken.isEof() ? startPos - 2 : startPos;
	// get position of beginning of the line
	while (lineStart > 0 && sourceCode[lineStart] != '\n')
		--lineStart;
	// get position of end of line
	size_t lineEnd = startPos;
	while (lineEnd != sourceCode.size() && sourceCode[lineEnd] != '\n')
		++lineEnd;
	// get the error context form source code
	auto lineLen = lineEnd - lineStart;
	// if the bad token was an EOF, remove trailing newline
	if (badToken.isEof()) --lineLen;
	auto context = sourceCode.substr(lineStart, lineLen);
	// print context
	out << context << std::endl;
	// get token location in relation to above line to underline it
	auto offset = startPos - lineStart;
	// if the bad token was an EOF, remove trailing newline
	if (badToken.isEof()) --offset;
	out << std::string(offset, ' ') << '~' << std::endl;
}
