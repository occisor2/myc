#pragma once

#include "ast.h"

// fixes recursive include problem
class Return;
class Expr;
class NumLit;
class BinExp;
class State;
class Decl;
class Ident;
class Compound;
class FuncDecl;

class Visitor
{
public:
	Visitor();
	~Visitor();
	virtual void visit(Expr& node) = 0;
	virtual void visit(NumLit& node) = 0;
	virtual void visit(BinExp& node) = 0;
	virtual void visit(State& node) = 0;
	virtual void visit(Ident& node) = 0;
	virtual void visit(Decl& node) = 0;
	virtual void visit(Compound& node) = 0;
	virtual void visit(FuncDecl& node) = 0;
	virtual void visit(Return& node) = 0;
};
