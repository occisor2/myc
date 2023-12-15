#pragma once

#include "AST/AST.h"
#include "Addr.h"
#include "Instruct.h"
#include "Symbol/SymTable.h"
#include <unordered_map>
#include <vector>

namespace IR
{

class Generator
{
	const std::unordered_map<AST::Type, Instruct::Operator> typeTable = {
		{AST::Type::Add, Instruct::Operator::Add},
		{AST::Type::Subtract, Instruct::Operator::Sub},
		{AST::Type::Multiply, Instruct::Operator::Mul},
		{AST::Type::Divide, Instruct::Operator::Div},
		{AST::Type::Assign, Instruct::Operator::Assign}
	};
	
public:
	Generator(SymTable& symTable);
	
	void operator()(const AST& ast);
	std::vector<Instruct> getLines() const;

	void debug();

private:
	SymTable& symTable;
	std::vector<Instruct> lines;

	Addr makeTemp();
	Addr expression(const AST::Node* n);
	Addr bin(AST::Type type, Addr left, Addr right);
	Addr assign(Addr dest, Addr arg1);
};

}
