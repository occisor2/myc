#pragma once

#include "ast.h"
#include <string>

class ReturnState;
class Expr;
class NumLit;
class BinExp;
class State;
class Decl;
class Ident;
class Compound;
class FuncDecl;

class NodeVisitor
{
public:
	NodeVisitor() {}
	~NodeVisitor() {}
	virtual void visit(Expr& node) = 0;
	virtual void visit(NumLit& node) = 0;
	virtual void visit(BinExp& node) = 0;
	virtual void visit(State& node) = 0;
	virtual void visit(Ident& node) = 0;
	virtual void visit(Decl& node) = 0;
	virtual void visit(Compound& node) = 0;
	virtual void visit(FuncDecl& node) = 0;
	virtual void visit(ReturnState& node) = 0;
};

class PrintAST : public NodeVisitor
{
public:
	PrintAST();

	void visit(Expr& node) override;
	void visit(NumLit& node) override;
	void visit(BinExp& node) override;
	void visit(State& node) override;
	void visit(Ident& node) override;
	void visit(Decl& node) override;
	void visit(Compound& node) override;
	void visit(FuncDecl& node) override;
	void visit(ReturnState& node) override;

private:
	const int indent = 2;
	int depth;

	void enterLevel();
	void leaveLevel();
	std::string indentString() const;
};
