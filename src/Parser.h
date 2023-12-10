#pragma once

#include "AST.h"
#include "Token.h"
#include <memory>
#include <stdexcept>
#include <vector>

class ParseError : std::runtime_error
{
public:
	ParseError(const char* message, unsigned int line) noexcept;
	const char* what() const noexcept override;

	std::string message;
	unsigned int line;
};

class Parser
{
	using Node = AST::Node;
	
public:
	explicit Parser(const std::vector<Token>& tokens);

	void parse();

private:
	const std::vector<Token>& tokens;
	std::vector<Token>::size_type pos;
	AST ast;

	Token consume();
	const Token& peek() const;
	void next();
	bool empty() const;
	
	void printAST();
	int interpret(const Node* node);
	
	std::unique_ptr<Node> primary();
	std::unique_ptr<Node> binExp(int prevPrec);

	std::pair<int, int> infixPrecedence(const Token& t) const;
};
