#include "AST.h"
#include <memory>

using Node = AST::Node;

Node::Node(Type type, int intLit)
	: type(type), intLit(intLit), left(nullptr), right(nullptr)
{}

Node::Node(Type type, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
	: type(type), left(std::move(left)), right(std::move(right))
{}

AST::AST(std::unique_ptr<Node> root)
	: root(std::move(root))
{}

