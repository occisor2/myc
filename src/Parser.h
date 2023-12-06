#pragma once

#include "AST.h"
#include "Token.h"
#include "Reader.h"
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
	Reader<std::vector<Token>::const_iterator> reader;
	AST ast;

	void printAST();
	int interpret(const Node* node);
	std::unique_ptr<Node> primary();
	std::unique_ptr<Node> binExp();
};
