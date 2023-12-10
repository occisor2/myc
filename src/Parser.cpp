#include "Parser.h"
#include "AST.h"
#include "Token.h"
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

Parser::Parser(const std::vector<Token>& tokens)
	: tokens(tokens), pos(0)
{}

Token Parser::consume()
{
	if (empty())
	{
		// Get the position of the last token to get the line
		auto line = pos == 0 ? 0 : tokens[pos - 1].line;
		throw ParseError("unexpected EOF", line);
	}
	
	return tokens[pos++];
}

const Token& Parser::peek() const
{
	if (empty())
	{
		// Get the position of the last token to get the line
		auto line = pos == 0 ? 0 : tokens[pos - 1].line;
		throw ParseError("unexpected EOF", line);
	}
	
	return tokens[pos];
}

void Parser::next()
{
	pos++;
}

bool Parser::empty() const
{
	return pos >= tokens.size();
}

void Parser::parse()
{
	if (empty())
		return;
	
	ast.root = binExp(0);

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
	auto tok = consume();

	switch (tok.type)
	{
	case Token::Type::IntLit:
		return std::make_unique<Node>(tok.type, tok.intLit, tok.line);
	default:
		throw ParseError("expected literal", tok.line);
	}
}

std::unique_ptr<Node> Parser::binExp(int prevPrec)
{
	auto left = primary();
	
	while (not empty())
	{
		auto op = peek();
		if (op.type == Token::Type::IntLit)
			throw ParseError("expected operator", op.line);

		auto [lbp, rbp] = infixPrecedence(op);
		if (lbp < prevPrec)
			break;

		next();
		auto right  = binExp(rbp);

		left = std::make_unique<Node>(op.type, op.line, std::move(left), std::move(right));
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
		throw ParseError("bad operator", t.line);
	}
}
