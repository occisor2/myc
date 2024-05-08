#pragma once

#include "diagnostic.h"
#include "symtable.h"
#include "visitor.h"

class NameError : public Diagnostic
{
public:
	NameError(Token badToken, std::string fileName, std::string msg)
		: Diagnostic(Diagnostic::Type::Error, badToken, fileName, msg) {}
};

class NameResolution : public Visitor
{
public:
	NameResolution(std::string fileName);

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
	SymTable* currentScope = nullptr;

	void error(Token badToken, std::string msg);
};
