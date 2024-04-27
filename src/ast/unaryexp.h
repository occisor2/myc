#pragma once

#include "expr.h"
#include <memory>

class UnaryExp : public Expr
{
public:
	enum class UnaryOp
	{
		Negate
	};
	
	UnaryExp() = delete;
	UnaryExp(UnaryOp unaryOp, Token token);

	UnaryOp unaryOp;
	std::unique_ptr<Expr> left;
};

