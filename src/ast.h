#pragma once

#include <memory>
#include <variant>

class NumLit;
class BinExp;

using Expr = std::variant<NumLit, BinExp>;

class NumLit
{
public:
	NumLit() = delete;
	NumLit(int numLit);

	int numLit;
};

class BinExp
{
public:
	enum class OpType
	{
		Sub,
		Add,
		Div,
		Mul
	};

	BinExp() = delete;
	BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);

	OpType opType;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
};
