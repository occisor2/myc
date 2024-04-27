#pragma once

#include "node.h"

class Expr : public Node
{
public:
	enum class Type
	{
		Unary,
		Binary
	};

	Expr() = delete;
	Expr(Type type);
	
	Type type;
};
