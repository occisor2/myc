#include "Parser.h"
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

using Node = AST::Node;

ParseError::ParseError(const char* message, unsigned int line) noexcept
	: std::runtime_error(message), message(message), line(line)
{}

const char* ParseError::what() const noexcept
{
	return std::runtime_error::what();
}

Parser::Parser(Scanner& scanner)
	: scanner(scanner)
{}

AST Parser::parse()
{
	ast.root = binExp(0);

	return std::move(ast);
}

std::unique_ptr<Node> Parser::primary()
{	
	auto tok = scanner.scan();

	switch (tok.type)
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(tok.type, tok.intLit);
	default:
		throw ParseError("expected literal", scanner.getLine());
	}
}

std::unique_ptr<Node> Parser::binExp(int prevPrec)
{
	// Scan in the left literal
	auto left = primary();
	
	// Scan the operator token
	scanner.scan();
	
	while (true)
	{
		auto op = scanner.peek();
		if (op.type == Token::Type::Eof)
			break;
		
		if (op.type == Token::Type::IntLit)
			throw ParseError("expected operator", scanner.getLine());

		auto [lbp, rbp] = infixPrecedence(op);
		if (lbp < prevPrec)
			break;

		auto right  = binExp(rbp);

		left = std::make_unique<Node>(op.type, std::move(left), std::move(right));
	}

	return left;
}

std::pair<int, int> Parser::infixPrecedence(const Token& t) const
{
	using Type = Token::Type;
	
	switch (t.type)
	{
	case Type::Plus:
	case Type::Minus:
		return std::make_pair(1, 2);
	case Type::Star:
	case Type::Slash:
		return std::make_pair(3, 4);
	default:
		throw ParseError("bad operator", scanner.getLine());
	}
}
