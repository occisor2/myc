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
	};
	
public:
	Generator(SymTable& symTable);
	
	std::vector<Instruct> operator()(const AST& ast);
	
private:
	SymTable& symTable;
	std::vector<Instruct> lines;

	Addr makeTemp();
	Addr expression(const AST::Node* n);
	Addr bin(AST::Type type, Addr left, Addr right);
};

}
