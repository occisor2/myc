#include "nameresolution.h"
#include <format>

NameResolution::NameResolution(std::string fileName)
	: fileName(fileName)
{}

void NameResolution::visit(Compound& node)
{
	SymTable newScope;
	currentScope = &newScope;

	for (auto& statement : node.statements)
		statement->accept(*this);
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
	throw NameError(badToken, fileName, msg);
}
