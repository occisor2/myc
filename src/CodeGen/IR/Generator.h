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
		{AST::Type::Assign, Instruct::Operator::Assign},
		{AST::Type::Equal, Instruct::Operator::Equal},
		{AST::Type::Not, Instruct::Operator::Not},
		{AST::Type::NotEqual, Instruct::Operator::NotEqual},
		{AST::Type::Greater, Instruct::Operator::Greater},
		{AST::Type::GreaterEqual, Instruct::Operator::GreaterEqual},
		{AST::Type::Less, Instruct::Operator::Less},
		{AST::Type::LessEqual, Instruct::Operator::LessEqual},
	};
	
public:
	Generator();

	void genExp(const AST& ast, SymTable* symTable);
	void debug();

private:
	SymTable* symTable;
	std::vector<Instruct> lines;
	
	Addr makeTemp();
	Addr expression(const AST::Node* n);
	Addr bin(AST::Type type, Addr left, Addr right); 
	Addr unary(AST::Type type, Addr arg1);
	Addr assign(Addr dest, Addr arg1);
};

}
