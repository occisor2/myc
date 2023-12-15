#include "Parser.h"
#include "AST/AST.h"
#include "CodeGen/IR/Generator.h"
#include "Lex/Token.h"
#include "error.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <utility>

using Node = AST::Node;

Parser::Parser(Scanner& scanner)
	: scanner(scanner), IRGen(symTable)
{}

void Parser::parse()
{
	statements();
	// auto lines = IRGen.getLines();

	// for (const auto& i : lines)
	// 	std::cout << i << std::endl;

	std::cout << "Parsed with no errors" << std::endl;
}

void Parser::panic(std::string message) const
{
	error::fatal(message, scanner.getFileName(), scanner.getLine());
}

void Parser::statements()
{
	// Scan in the first token
	scanner.scan();
	
	for(;;)
	{
		std::unique_ptr<Node> statementNode;
		auto t = scanner.peek();

		switch (t.getType())
		{
		case Token::Type::Eof:
			return;
		case Token::Type::Semi: // Null statement (just a semi-colon)
			// East the semi
			matchSemi();
			break;
		case Token::Type::Int:
			// Will be nullptr if declaration ended up not being an
			// assign as well.
			statementNode = varDeclare();
			break;
		default: // Parse an 'expression statement'
			statementNode = expression(0);
		}

		if (statementNode)
		{
			auto ast = AST(std::move(statementNode));
			ast.debug();
		}

		// if (statementNode)
		// 	IRGen(AST(std::move(statementNode)));
	}
}

std::unique_ptr<Node> Parser::varDeclare()
{
	// Match correct sequence of tokens
	match(Token::Type::Int, "int");
	auto ident = matchIdent();
	matchSemi();

	// Make sure the symbol doesn't already exist
	if (not symTable.exists(ident.getIdent()))
		symTable.insert(ident.getIdent());
	else
		panic(std::format("variable '{}' already declared", ident.getIdent()));

	return nullptr;
}

std::unique_ptr<Node> Parser::primary()
{	
	auto t = scanner.peek();

	switch (t.getType())
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(AST::Type::IntLit, t.getIntLit());
	case Token::Type::Ident:
		// Make sure the identifier has been declared
		if (not symTable.exists(t.getIdent()))
			panic(std::format("variable '{}' has not been declared", t.getIdent()));
		return std::make_unique<Node>(AST::Type::Ident, t.getIdent());
	default:
		panic(std::format("expected integer literal but got '{}' instead", t.getText()));
	}
}

std::unique_ptr<Node> Parser::expression(int prevPrec)
{
	auto left = primary();
	
	// Scan the operator token
	scanner.scan();

	if (scanner.peek().getType() == Token::Type::Semi)
		return left;
	
	while (true)
	{
		auto op = scanner.peek();
		
		if (op.getType() == Token::Type::Eof)
			break;
			
		auto [lbp, rbp] = infixPrecedence(op);
		if (lbp < prevPrec)
			break;

		scanner.scan(); // Bump past operator
		auto right  = expression(rbp);

		// If this is an assignment, check the left side is an lvalue
		// and swap the nodes (needed for the generator).
		if (op.getType() == Token::Type::Equals)
		{
			if (left->type != AST::Type::Ident)
				panic("invalid lvalue");
			left.swap(right);
		}

		left = std::make_unique<Node>(tokenToBinOpType(op), std::move(left), std::move(right));

		if (scanner.peek().getType() == Token::Type::Semi)
			return left;
	}

	return left;
}

std::pair<int, int> Parser::infixPrecedence(const Token& t) const
{
	using Type = Token::Type;
	
	switch (t.getType())
	{
	case Type::Equals:
		return std::make_pair(2, 1);
	case Type::Plus:
	case Type::Minus:
		return std::make_pair(10, 11);
	case Type::Star:
	case Type::Slash:
		return std::make_pair(12, 13);
	default:
		panic(std::format("invalid operator '{}'", t.getText()));
	}
}

AST::Type Parser::tokenToBinOpType(const Token& t) const
{
	switch (t.getType())
	{
	case Token::Type::Equals:
		return AST::Type::Assign;
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

void Parser::match(Token::Type type, const std::string& expected)
{
	auto what = scanner.peek();
	if (what.getType() != type)
		panic(std::format("expected '{}' got '{}'", expected, what.getText()));
	else
		scanner.scan();
}

void Parser::matchSemi()
{
	match(Token::Type::Semi, ";");
}

Token Parser::matchIdent()
{
	auto ident = scanner.peek();
	match(Token::Type::Ident, "identifier");
	return ident;
}
