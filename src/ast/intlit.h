#pragma once

#include "ast/expr.h"

class IntLit : public Expr
{
public:
	IntLit() = delete;
	IntLit(int intLit, Token token);
};
