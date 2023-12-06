#include "Parser.h"
#include "AST.h"
#include "Reader.h"
#include "Token.h"
#include <memory>
#include <stdexcept>

using Node = AST::Node;

ParseError::ParseError(const char* message, unsigned int line) noexcept
	: std::runtime_error(message), message(message), line(line)
{}

const char* ParseError::what() const noexcept
{
	return std::runtime_error::what();
}

Parser::Parser(const std::vector<Token>& tokens)
	: reader(tokens.begin(), tokens.end())
{}

void Parser::parse()
{
	if (reader.isEmpty())
		return;
	
	ast.root = binExp();

	printAST();
}

void Parser::printAST()
{
	if (ast.root)
	{
		auto result = interpret(ast.root.get());
		std::cout << result << std::endl;
	}
}

int Parser::interpret(const Node* node)
{
	int leftVal{}, rightVal{};
	
	if (node->left)
		leftVal = interpret(node->left.get());
	if (node->right)
		rightVal = interpret(node->right.get());

	switch (node->type)
	{
	case AST::Type::Add:
		return leftVal + rightVal;
	case AST::Type::Subtract:
		return leftVal - rightVal;
	case AST::Type::Multiply:
		return leftVal * rightVal;
	case AST::Type::Divide:
		return leftVal / rightVal;
	case AST::Type::IntLit:
		return node->intLit;
	default:
		throw ParseError("unknown operator", node->line);
	}
}

std::unique_ptr<Node> Parser::primary()
{
	auto tok = reader.consume();

	switch (tok.type)
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(tok.type, tok.intLit);
	default:
		throw ParseError("syntax error", tok.line);
	}
}

std::unique_ptr<Node> Parser::binExp()
{
	auto left = primary();

	// If no more tokens, just return the left node.
	if (reader.isEmpty())
		return left;

	// Read in the operator token.
	auto op = reader.consume();

	auto right = binExp();

	return std::make_unique<Node>(op.type, op.line, std::move(left), std::move(right));
}
