#pragma once

#include "symtable.h"
#include "visitor.h"

class NameResolution : public Visitor
{
public:
	NameResolution(std::string fileName, std::string_view sourceCode);

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
	std::string fileName;
	std::string_view sourceCode;
	bool hasErrors = false;
	SymTable* currentScope = nullptr;

	void error(Token badToken, std::string msg);
};
