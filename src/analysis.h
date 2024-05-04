#pragma once

#include "ast.h"
#include <string>

class Expr;
class NumLit;
class BinExp;

class NodeVisitor
{
public:
	NodeVisitor() {}
	~NodeVisitor() {}
	virtual void visit(Expr& node) = 0;
	virtual void visit(NumLit& node) = 0;
	virtual void visit(BinExp& node) = 0;
};

class PrintAST : public NodeVisitor
{
public:
	PrintAST();

	void visit(Expr& node) override;
	void visit(NumLit& node) override;
	void visit(BinExp& node) override;

private:
	const int indent = 2;
	int depth;

	void enterLevel();
	void leaveLevel();
	std::string identString() const;
};
