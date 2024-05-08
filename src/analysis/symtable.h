#pragma once

#include <string>
#include <unordered_map>

class Symbol
{
public:
	Symbol();
	Symbol(std::string name);

	std::string name;
};

class SymTable
{
public:
	SymTable(SymTable* prev = nullptr);


	bool lookup(std::string name);
	void insert(Symbol symbol);

private:
	SymTable* prev;
	std::unordered_map<std::string, Symbol> table;
};
