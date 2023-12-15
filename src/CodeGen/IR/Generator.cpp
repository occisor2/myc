#include "Generator.h"
#include "AST/AST.h"
#include "Instruct.h"
#include "Addr.h"
#include "Symbol/SymTable.h"
#include "Symbol/Symbol.h"
#include <charconv>
#include <format>
#include <stdexcept>
#include <string>

using namespace IR;

Generator::Generator(SymTable& symTable)
	: symTable(symTable)
{}

void Generator::operator()(const AST& ast)
{
	expression(ast.root.get());
}

Addr Generator::makeTemp()
{
	auto temp = Symbol(std::to_string(lines.size()));
	symTable.insert(temp);
	return Addr(temp.getName(), true);
}

Addr Generator::expression(const AST::Node* n)
{
	Addr left, right;
	
	if (n->left) left = expression(n->left.get());
	if (n->right) right = expression(n->right.get());

	switch (n->type)
	{
	case AST::Type::IntLit:
		return Addr(n->intLit);
	case AST::Type::Add:
	case AST::Type::Subtract:
	case AST::Type::Multiply:
	case AST::Type::Divide:
		return bin(n->type, left, right);
	default:
		throw std::runtime_error("bad operator");
	}
}

Addr Generator::bin(AST::Type type, Addr left, Addr right)
{
	// Convert the AST type to an IR operator
	Instruct::Operator op = typeTable.at(type);
	// Create a temporary to hold result of operation
	auto result = makeTemp();

	auto newInst = Instruct(op, left, right, result);
	lines.push_back(newInst);

	return result;
}
