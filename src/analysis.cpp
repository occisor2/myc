#include "analysis.h"
#include <iostream>
#include <variant>

void PrintAST::visit(std::unique_ptr<Expr>& expr)
{
	std::visit(*this, *expr);
}

void PrintAST::operator()(NumLit& numLit)
{
	std::cout << numLit.numLit;
}

void PrintAST::operator()(BinExp& binExp)
{
	std::cout << "(";
	visit(binExp.left);

	switch (binExp.opType)
	{
	case BinExp::OpType::Sub:
		std::cout << '-';
		break;
	case BinExp::OpType::Add:
		std::cout << '+';
		break;
	case BinExp::OpType::Div:
		std::cout << '/';
		break;
	case BinExp::OpType::Mul:
		std::cout << '*';
		break;
	}

        visit(binExp.right);
	std::cout << ")";
}
