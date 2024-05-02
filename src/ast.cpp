#include "ast.h"

NumLit::NumLit(int numLit)
	: numLit(numLit)
{}

BinExp::BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
	: opType(opType), left(std::move(left)), right(std::move(right))
{}
