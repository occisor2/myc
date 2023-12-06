#pragma once

#include "Token.h"
#include <memory>
#include <vector>

class AST
{
public:
	enum class Type
	{
		Add,
		Subtract,
		Multiply,
		Divide,
		IntLit
	};

	class Node
	{
	public:
		Type type;
		int intLit{};
		unsigned int line;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node() = default;
		Node(Type type, int intLit, unsigned int line);
		Node(Token::Type type, int intLit, unsigned int line);
		Node(Type type, unsigned int line, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr);
		Node(Token::Type type, unsigned int line, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr);

	private:
		Type tokenToNodeType(Token::Type t) const;
	};

	std::unique_ptr<Node> root;

	explicit AST(std::unique_ptr<Node> root = nullptr);
};
