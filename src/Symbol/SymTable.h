#pragma once

#include "Symbol.h"
#include <string>
#include <unordered_map>

class SymTable
{
public:
	SymTable(SymTable* prev = nullptr);

	void insert(Symbol s);
	Symbol lookup(const std::string& name) const;
	bool exists(const std::string& name) const;

private:
	std::unordered_map<std::string, Symbol> table;
	SymTable* prev;
};

