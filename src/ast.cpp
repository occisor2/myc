#include "ast.h"
#include <cassert>
#include <string>

State::State(StateType stateType)
	: stateType(stateType)
{}

void State::accept(NodeVisitor& visitor)
{
	switch (stateType)
	{
	case StateType::Expr:
		static_cast<Expr*>(this)->accept(visitor);
		break;
	case StateType::Decl:
		static_cast<Decl*>(this)->accept(visitor);
		break;
	}
}

Ident::Ident(std::string name)
	: Expr(ExpType::Ident), name(name)
{}

void Ident::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

Decl::Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right)
	: State(StateType::Decl), ident(std::move(ident)), right(std::move(right))
{}

void Decl::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

Expr::Expr(ExpType expType)
	: State(StateType::Expr), expType(expType)
{}

void Expr::accept(NodeVisitor& visitor)
{
	switch (expType)
	{
	case ExpType::NumLit:
		static_cast<NumLit*>(this)->accept(visitor);
		break;
	case ExpType::Ident:
		static_cast<Ident*>(this)->accept(visitor);
		break;
	case ExpType::Bin:
		static_cast<BinExp*>(this)->accept(visitor);
		break;
	default:
		assert(((void)"unreachable", false));
	}
}

NumLit::NumLit(int numLit)
	: Expr(ExpType::NumLit), numLit(numLit)
{}

void NumLit::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

BinExp::BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
	: Expr(ExpType::Bin), opType(opType), left(std::move(left)),
	  right(std::move(right))
{}

void BinExp::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}
