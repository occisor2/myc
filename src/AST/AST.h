#pragma once

#include <memory>
#include <string>
#include <vector>

class AST
{
public:
	enum class Type
	{
		IntLit,
		Ident,
		Lval,
		Assign,
		Add,
		Subtract,
		Multiply,
		Divide,
	};

	class Node
	{
	public:
		Type type{};
		int intLit{};
		std::string symName;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node() = default;
		Node(Type type, int intLit);
		Node(Type type, std::string ident);
		Node(Type type, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr);
	};

	std::unique_ptr<Node> root;

	explicit AST(std::unique_ptr<Node> root = nullptr);

	void debug() const;

private:
	void recDebug(const Node* n, int level) const;
};
