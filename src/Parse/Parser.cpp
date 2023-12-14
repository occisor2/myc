#include "Parser.h"
#include "AST/AST.h"
#include "CodeGen/IR/Generator.h"
#include "Lex/Token.h"
#include "error.h"
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

using Node = AST::Node;

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

void Parser::panic(std::string message) const
{
	error::fatal(message, scanner.getFileName(), scanner.getLine());
}

std::unique_ptr<Node> Parser::primary()
{	
	auto t = scanner.scan();

	switch (t.getType())
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(AST::Type::IntLit, t.getIntLit());
	default:
		panic(std::format("expected integer literal but got '{}' instead", t.getText()));
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
		if (op.getType() == Token::Type::Eof)
			break;

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
	
	switch (t.getType())
	{
	case Type::Plus:
	case Type::Minus:
		return std::make_pair(1, 2);
	case Type::Star:
	case Type::Slash:
		return std::make_pair(3, 4);
	default:
		panic(std::format("invalid operator '{}'", t.getText()));
	}
}

AST::Type Parser::tokenToBinOpType(const Token& t) const
{
	switch (t.getType())
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
