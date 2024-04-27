#pragma once

#include "expr.h"
#include "token.h"
#include <memory>

class BinExp : public Expr
{
public:
	enum class BinOp
	{
		Add,
		Sub,
		Div,
		Mul
	};
	
	BinExp() = delete;
	BinExp(BinOp binOp, Token token);

	BinOp binOp;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
};
