#include "ast.h"
#include <cassert>

Expr::Expr(Type type)
	: type(type)
{}

void Expr::accept(NodeVisitor& visitor)
{
	switch (type)
	{
	case Type::NumLit:
		static_cast<NumLit*>(this)->accept(visitor);
		break;
	case Type::Bin:
		static_cast<BinExp*>(this)->accept(visitor);
		break;
	default:
		assert(((void)"unreachable", false));
	}
}

NumLit::NumLit(int numLit)
	: Expr(Type::NumLit), numLit(numLit)
{}

void NumLit::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

BinExp::BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
	: Expr(Type::Bin), opType(opType), left(std::move(left)),
	  right(std::move(right))
{}

void BinExp::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}
