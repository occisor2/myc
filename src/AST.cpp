#include "AST.h"
#include "Token.h"
#include <memory>
#include <unordered_map>

using Node = AST::Node;

Node::Node(Type type, int intLit)
	: type(type), intLit(intLit), left(nullptr), right(nullptr)
{}

Node::Node(Token::Type type, int intLit)
	: Node(tokenToNodeType(type), intLit)
{}

Node::Node(Type type, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
	: type(type), left(std::move(left)), right(std::move(right))
{}

Node::Node(Token::Type type, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
	: Node::Node(tokenToNodeType(type), std::move(left), std::move(right))
{}

AST::Type Node::tokenToNodeType(Token::Type t)
{
	Type convert{};

	switch (t)
	{
	case Token::Type::Plus:
		convert = Type::Add;
		break;
	case Token::Type::Minus:
		convert = Type::Subtract;
		break;
	case Token::Type::Star:
		convert = Type::Multiply;
		break;
	case Token::Type::Slash:
		convert = Type::Divide;
		break;
	case Token::Type::IntLit:
		convert = Type::IntLit;
		break;
	default:
		break;
	}
	
	return convert;
}

AST::AST(std::unique_ptr<Node> root)
	: root(std::move(root))
{}

