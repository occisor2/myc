#include "Parser.h"
#include "AST/AST.h"
#include "CodeGen/IR/Generator.h"
#include "Lex/Token.h"
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

void Parser::parse()
{

	AST ast(expression(0));
	IR::Generator gen(symTable);
	auto insts = gen(ast);

	for (const auto& i : insts)
		std::cout << i << std::endl;
}

std::unique_ptr<Node> Parser::primary()
{	
	auto tok = scanner.scan();

	switch (tok.type)
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(AST::Type::IntLit, tok.intLit);
	default:
		throw ParseError("expected literal", scanner.getLine());
	}
}

std::unique_ptr<Node> Parser::expression(int prevPrec)
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

		auto right  = expression(rbp);

		left = std::make_unique<Node>(tokenToBinOpType(op), std::move(left), std::move(right));
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

AST::Type Parser::tokenToBinOpType(const Token& t) const
{
	switch (t.type)
	{
	case Token::Type::Plus:
		return AST::Type::Add;
	case Token::Type::Minus:
		return AST::Type::Subtract;
	case Token::Type::Star:
		return AST::Type::Multiply;
	case Token::Type::Slash:
		return AST::Type::Divide;
	default:
		throw std::runtime_error("bad operator");
	}
}
