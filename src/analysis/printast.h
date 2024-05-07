#pragma once

#include "visitor.h"
#include <string>

class PrintAST : public Visitor
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
	void visit(Return& node) override;

private:
	const int indent = 2;
	int depth;

	void enterLevel();
	void leaveLevel();
	std::string indentString() const;
};
