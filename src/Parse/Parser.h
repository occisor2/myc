#pragma once

#include "AST/AST.h"
#include "CodeGen/IR/Generator.h"
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
	IR::Generator IRGen;

	[[noreturn]] void panic(std::string message) const;

	void statements();
	std::unique_ptr<Node> varDeclare();
	std::unique_ptr<Node> primary();
	std::unique_ptr<Node> expression(int prevPrec);

	std::pair<int, int> infixPrecedence(const Token& t) const;
	AST::Type tokenToBinOpType(const Token& t) const;

	/**
	 * Test current token against type.
	 *
	 * If successfully, scans the next token, else panic.
	 */
	void match(Token::Type type, const std::string& expected);
	void matchSemi();
	Token matchIdent();
};
