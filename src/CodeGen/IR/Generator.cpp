#include "Generator.h"
#include "AST/AST.h"
#include "Instruct.h"
#include "Addr.h"
#include "Symbol/SymTable.h"
#include <charconv>
#include <iostream>
#include <string>

using namespace IR;

Generator::Generator(SymTable& symTable)
	: symTable(symTable)
{}

void Generator::operator()(const AST& ast)
{
	block(ast);
}

std::vector<Instruct> Generator::getLines() const
{
	return lines;
}

void Generator::block(const AST& block)
{
	auto& statements = block.root->block;

	for (auto& s : statements)
		expression(s.get());
}

Addr Generator::makeTemp()
{
	// Generate a temporary symbol with a number as a name. User
	// declared variables can't start with numbers, so this should
	// prevent clashses.
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
	case AST::Type::Ident:
		return Addr(n->symName);
	case AST::Type::Assign:
		return assign(right, left);
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

Addr Generator::assign(Addr dest, Addr arg1)
{
	Instruct::Operator op = Instruct::Operator::Assign;

	auto newInst = Instruct(op, dest, arg1);
	lines.push_back(newInst);

	return dest;
}

void Generator::debug()
{
	std::cout << "Op\t" <<  "Arg1\t" << "Arg2\t" << "Result" << std::endl;

	for (const auto& l : lines)
	{
		std::cout << l.opTextTable.at(l.op) << "\t";
		std::cout << l.arg1 << "\t";
		std::cout << l.arg2 << "\t";
		std::cout << l.result << std::endl;
	}
}
