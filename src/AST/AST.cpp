#include "AST.h"
#include <iostream>
#include <memory>

using Node = AST::Node;

Node::Node(Type type, int intLit)
	: type(type), intLit(intLit), left(nullptr), right(nullptr)
{}

Node::Node(Type type, std::string ident)
	: type(type), symName(ident)
{}

Node::Node(Type type, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
	: type(type), left(std::move(left)), right(std::move(right))
{}

AST::AST(std::unique_ptr<Node> root)
	: root(std::move(root))
{}

void AST::debug() const
{
	recDebug(root.get(), 0);
}

void AST::recDebug(const Node* n, int level) const
{
	if (n->left.get())
		recDebug(n->left.get(), level+2);
	if (n->right.get())
		recDebug(n->right.get(), level+2);

	for (auto i = 0; i < level; ++i) std::cout << " ";
	
	switch (n->type)
	{
	case Type::IntLit:
		std::cout << "INTLIT: ";
		std::cout << n->intLit;
		std::cout << std::endl;
		break;
	case Type::Ident:
		std::cout << "IDENT: ";
		std::cout << n->symName;
		std::cout << std::endl;
		break;
	case Type::Lval:
		std::cout << "LVAL: ";
		std::cout << n->symName;
		std::cout << std::endl;
		break;
	case Type::Assign:
		std::cout << "ASSIGN: ";
		std::cout << std::endl;
		break;
	case Type::Add:
		std::cout << "ADD: ";
		std::cout << std::endl;
		break;
	case Type::Subtract:
		std::cout << "SUB: ";
		std::cout << std::endl;
		break;
	case Type::Multiply:
		std::cout << "MUL: ";
		std::cout << std::endl;
		break;
	case Type::Divide:
		std::cout << "DIV: ";
		std::cout << std::endl;
		break;
	}
}
