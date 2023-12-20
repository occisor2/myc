#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class AST
{
public:
	enum class Type
	{
		Block,
		IntLit,
		Ident,
		Assign,
		Add,
		Subtract,
		Multiply,
		Divide,
		Equal,
		Less,
		Greater,
		LessEqual,
		GreaterEqual,
		Not,
		NotEqual,
		Neg
	};

	class Node
	{
	public:
		Type type{};
		int intLit{};
		std::string symName;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
		std::vector<std::unique_ptr<Node>> block;

		Node() = default;
		Node(Type type, int intLit);
		Node(Type type, std::string ident);
		Node(Type type, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr);
		explicit Node(std::vector<std::unique_ptr<Node>> block);
	};

	std::unique_ptr<Node> root;

	AST(std::unique_ptr<Node> root = nullptr);

	void debug() const;

private:
	void recDebug(const Node* n, int level) const;
};
