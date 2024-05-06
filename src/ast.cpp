#include "ast.h"
#include "token.h"
#include <cassert>
#include <string>

NodeBase::NodeBase(Token token)
	: token(token)
{}

State::State(StateType stateType, Token token)
	: NodeBase(token), stateType(stateType)
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

Ident::Ident(std::string name, Token token)
	: Expr(ExpType::Ident, token), name(name)
{}

void Ident::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

Decl::Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right, Token token)
	: State(StateType::Decl, token),
	  ident(std::move(ident)), right(std::move(right))
{}

void Decl::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

Expr::Expr(ExpType expType, Token token)
	: State(StateType::Expr, token), expType(expType)
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

NumLit::NumLit(int numLit, Token token)
	: Expr(ExpType::NumLit, token), numLit(numLit)
{}

void NumLit::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}

BinExp::BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
			   Token token)
	: Expr(ExpType::Bin, token), opType(opType), left(std::move(left)),
	  right(std::move(right))
{}

void BinExp::accept(NodeVisitor& visitor)
{
	visitor.visit(*this);
}
