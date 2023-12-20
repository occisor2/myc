#include "Parser.h"
#include "AST/AST.h"
#include "CodeGen/IR/Generator.h"
#include "CodeGen/Targets/X86.h"
#include "Lex/Token.h"
#include "Symbol/SymTable.h"
#include "Symbol/Symbol.h"
#include "error.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <utility>
#include <vector>

using Node = AST::Node;

Parser::Parser(Scanner& scanner)
	: scanner(scanner)
{}

void Parser::parse()
{
	// Scan the first token
	scanner.scan();

	block();

	IRGen.debug();
}

void Parser::panic(std::string message) const
{
	error::fatal(message, scanner.getFileName(), scanner.getLine());
}



void Parser::flowControl()
{
	match(Token::Type::If, "if");
	match(Token::Type::OpenParen, "(");

	auto compExp = expression(0);
	
	match(Token::Type::CloseParen, ")");
		
}

void Parser::block()
{
	match(Token::Type::OpenBrace, "{");

	// Save previous scope
	auto lastScope = symTable;
	// Create a new scope
	symTable = SymTable(&lastScope);

	while (scanner.peek().getType() != Token::Type::CloseBrace)
	{
		if (scanner.eof())
			panic("unexpected EOF");

		auto s = statement();
		if (s)
			IRGen.genExp(AST(std::move(s)), &symTable);
	}

	// Eat the closing brace
	match(Token::Type::CloseBrace, "}");
	// Restore previous scope
	symTable = lastScope;
}

std::unique_ptr<Node> Parser::statement()
{
	auto t = scanner.peek();

	switch (t.getType())
	{
	case Token::Type::Semi: // Null statement (just a semi-colon)
		// East the semi
		matchSemi();
		break;
	case Token::Type::OpenBrace:
		block();
		break;
	case Token::Type::Int:
		varDeclare();
		break;
	default:
		auto exp = expression(0);
		// Eat the trailing semi from the expression
		matchSemi();
		return exp;
	}

	return nullptr;
}

void Parser::varDeclare()
{
	// Match correct sequence of tokens
	match(Token::Type::Int, "int");
	auto ident = matchIdent();
	matchSemi();

	// Make sure the symbol doesn't already exist
	if (not symTable.exists(ident.getIdent()))
		symTable.insert(Symbol(ident.getIdent(), Symbol::Type::Local));
	else
		panic(std::format("variable '{}' already declared", ident.getIdent()));
}

std::unique_ptr<Node> Parser::primary()
{	
	auto t = scanner.next();

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
		if (unaryOpTable.contains(t.getType()))
		{
			auto [_, rbp] = prefixPrecedence(t);
			auto op = tokenToUnOpType(t);
			return std::make_unique<Node>(op, expression(rbp));
		}
		panic(std::format("expected expression but got '{}' instead", t.getText()));
	}
}

std::unique_ptr<Node> Parser::expression(int prevPrec)
{
	auto left = primary();

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

		scanner.next(); // Bump past operator1
		auto right  = expression(rbp);

		// If this is an assignment, check the left side is an lvalue
		// and swap the nodes (needed for the generator).
		if (op.getType() == Token::Type::Assign)
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
	case Type::Assign:
		return std::make_pair(2, 1);
	case Type::Equal:
	case Type::NotEqual:
		return std::make_pair(4, 5);
	case Type::Less:
	case Type::Greater:
	case Type::LessEqual:
	case Type::GreaterEqual:
		return std::make_pair(6, 7);
	case Type::Plus:
	case Type::Minus:
		return std::make_pair(10, 11);
	case Type::Star:
	case Type::Slash:
		return std::make_pair(12, 13);
	default:
		panic(std::format("not a binary operator '{}'", t.getText()));
	}
}

std::pair<int, int> Parser::prefixPrecedence(const Token& t) const
{
	using Type = Token::Type;
	
	switch (t.getType())
	{
	case Type::Not:
	case Type::Minus:
		return std::make_pair(0, 20);
	default:
		panic(std::format("not a unary operator '{}'", t.getText()));
	}
}

AST::Type Parser::tokenToBinOpType(const Token& t) const
{
	return binOpTable.at(t.getType());
}

AST::Type Parser::tokenToUnOpType(const Token& t) const
{
	return unaryOpTable.at(t.getType());
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
