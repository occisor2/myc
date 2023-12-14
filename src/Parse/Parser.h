#pragma once

#include "AST/AST.h"
#include "Lex/Scanner.h"
#include "Lex/Token.h"
#include "Symbol/SymTable.h"
#include <memory>
#include <string>
#include <unordered_map>

class Parser
{
	using Node = AST::Node;

	const std::unordered_map<Token::Type, AST::Type> opTable = {
		{Token::Type::Plus, AST::Type::Add},
		{Token::Type::Minus, AST::Type::Subtract},
		{Token::Type::Star, AST::Type::Multiply},
		{Token::Type::Slash, AST::Type::Divide}
	};
	
public:
	explicit Parser(Scanner& scanner);

	void parse();

private:
	Scanner& scanner;
	SymTable symTable;
	AST ast;

	[[noreturn]] void panic(std::string message) const;
	
	std::unique_ptr<Node> primary();
	std::unique_ptr<Node> expression(int prevPrec);

	std::pair<int, int> infixPrecedence(const Token& t) const;
	AST::Type tokenToBinOpType(const Token& t) const;
};
