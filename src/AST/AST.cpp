#include "AST.h"
#include "Lex/Token.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

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

Node::Node(std::vector<std::unique_ptr<Node>> block)
	: type(Type::Block), block(std::move(block))
{
	
}

void AST::debug() const
{
	recDebug(root.get(), 0);
}

void AST::recDebug(const Node* n, int level) const
{
	if (n->type != Type::Block)
	{
		if (n->left.get())
			recDebug(n->left.get(), level+2);
		if (n->right.get())
			recDebug(n->right.get(), level+2);
	}

	for (auto i = 0; i < level; ++i) std::cout << " ";
	
	switch (n->type)
	{
	case Type::Block:
		for (auto& i : n->block)
			recDebug(i.get(), level+2);
		std::cout << "BLOCK:" << std::endl;
		break;
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
	case Type::Equal:
		std::cout << "EQUAL: ";
		std::cout << std::endl;
		break;
	case Type::Less:
		std::cout << "LESS: ";
		std::cout << std::endl;
		break;
	case Type::Greater:
		std::cout << "GREATER: ";
		std::cout << std::endl;
		break;
	case Type::LessEqual:
		std::cout << "LESSEQUAL: ";
		std::cout << std::endl;
		break;
	case Type::GreaterEqual:
		std::cout << "GREATEREQUAL: ";
		std::cout << std::endl;
		break;
	case Type::Not:
		std::cout << "NOT: ";
		std::cout << std::endl;
		break;
	case Type::NotEqual:
		std::cout << "NOTEQUAL: ";
		std::cout << std::endl;
		break;
	case Type::Neg:
		std::cout << "NEG: ";
		std::cout << std::endl;		
		break;
	}
}
