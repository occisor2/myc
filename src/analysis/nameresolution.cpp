#include "nameresolution.h"
#include "diagnostic.h"
#include <format>
#include <iostream>

NameResolution::NameResolution(std::string fileName, std::string_view sourceCode)
	: fileName(fileName), sourceCode(sourceCode)
{}

void NameResolution::visit(Compound& node)
{
	// uses the stack to handle scoping
	auto oldScope = currentScope;
	SymTable newScope(currentScope);
	currentScope = &newScope;

	for (auto& statement : node.statements)
		statement->accept(*this);

	// restore old scope on function exit
	currentScope = oldScope;
}

void NameResolution::visit(Ident& node)
{
	if (not currentScope->lookup(node.name))
		error(node.token, std::format("unknown identifier '{}'", node.name));
}

void NameResolution::visit(Expr& node)
{
	node.accept(*this);
}

void NameResolution::visit(BinExp& node)
{
	node.left->accept(*this);
	node.right->accept(*this);
}

void NameResolution::visit(State& node)
{
	node.accept(*this);
}

void NameResolution::visit(Decl& node)
{
	if (currentScope->lookup(node.ident->name))
		error(node.token, std::format("identifier '{}' has already been declared", node.ident->name));
	else
		currentScope->insert(Symbol(node.ident->name));

	node.right->accept(*this);
}

void NameResolution::visit(FuncDecl& node)
{
	node.body->accept(*this);
}

void NameResolution::visit(Return& node)
{
	node.value->accept(*this);
}

void NameResolution::visit([[maybe_unused]] NumLit& node)
{
	return;
}

void NameResolution::error(Token badToken, std::string msg)
{
	Diagnostic d(Diagnostic::Type::Error, badToken, fileName, msg);
	d.print(std::cout, sourceCode);
}
