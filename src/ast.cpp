#include "ast.h"
#include <cassert>
#include <string>
#include <utility>

NodeBase::NodeBase(Token token)
	: token(token)
{}

State::State(StateType stateType, Token token)
	: NodeBase(token), stateType(stateType)
{}

void State::accept(Visitor& visitor)
{
	switch (stateType)
	{
	case StateType::Compound:
		static_cast<Compound*>(this)->accept(visitor);
		break;
	case StateType::Expr:
		static_cast<Expr*>(this)->accept(visitor);
		break;
	case StateType::Decl:
		static_cast<Decl*>(this)->accept(visitor);
		break;
	case StateType::Return:
		static_cast<Return*>(this)->accept(visitor);
		break;
	}
}

Return::Return(std::unique_ptr<Expr> value, Token token)
	: State(StateType::Return, token), value(std::move(value))
{}

void Return::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

Ident::Ident(std::string name, Token token)
	: Expr(ExpType::Ident, token), name(name)
{}

void Ident::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

Decl::Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right, Token token)
	: State(StateType::Decl, token),
	  ident(std::move(ident)), right(std::move(right))
{}

void Decl::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

Expr::Expr(ExpType expType, Token token)
	: State(StateType::Expr, token), expType(expType)
{}

void Expr::accept(Visitor& visitor)
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

void NumLit::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

BinExp::BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
			   Token token)
	: Expr(ExpType::Bin, token), opType(opType), left(std::move(left)),
	  right(std::move(right))
{}

void BinExp::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

Compound::Compound(std::vector<std::unique_ptr<State>> statements, Token token)
	: State(StateType::Compound, token), statements(std::move(statements))
{}

void Compound::accept(Visitor& visitor)
{
	visitor.visit(*this);
}

FuncDecl::FuncDecl(std::unique_ptr<Ident> name, std::unique_ptr<Compound> body, Token token)
	: NodeBase(token), name(std::move(name)), body(std::move(body))
{}

void FuncDecl::accept(Visitor& visitor)
{
	visitor.visit(*this);
}
