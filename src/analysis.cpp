#include "analysis.h"
#include <iostream>
#include <ostream>

PrintAST::PrintAST()
	: depth(0)
{}

void PrintAST::visit(Expr& node)
{
	node.accept(*this);
}

void PrintAST::visit(NumLit& node)
{
	std::cout << indentString() << "Int Literal: " << node.numLit << std::endl;
}

void PrintAST::visit(BinExp& node)
{
	std::cout << indentString() << "Binary Expression: ";

	switch (node.opType)
	{
	case BinExp::OpType::Sub:
		std::cout << "-";
		break;
	case BinExp::OpType::Add:
		std::cout << "+";
		break;
	case BinExp::OpType::Div:
		std::cout << "/";
		break;
	case BinExp::OpType::Mul:
		std::cout << "*";
		break;
	}

	std::cout << std::endl;

	enterLevel();
	node.left->accept(*this);
	leaveLevel();

	enterLevel();
	node.right->accept(*this);
	leaveLevel();
}

void PrintAST::visit(State& node)
{
	node.accept(*this);
}

void PrintAST::visit(Ident& node)
{
	std::cout << indentString() << "Identifier: " << node.name << std::endl;
}

void PrintAST::visit(Decl& node)
{
	std::cout << indentString() << "Var Declaration:" << std::endl;

	enterLevel();
	node.ident->accept(*this);
	leaveLevel();

	enterLevel();
	node.right->accept(*this);
	leaveLevel();
}

void PrintAST::enterLevel()
{
	depth += indent;
}

void PrintAST::leaveLevel()
{
	depth -= indent;
}

std::string PrintAST::indentString() const
{
	return std::string(depth, ' ');
}
