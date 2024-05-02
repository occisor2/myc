#pragma once

#include "ast.h"
#include <memory>

class PrintAST
{
public:
	void visit(std::unique_ptr<Expr>& expr);
	void operator()(NumLit& numLit);
	void operator()(BinExp& binExp);
};
