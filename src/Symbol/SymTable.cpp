#include "SymTable.h"
#include "Symbol.h"
#include <algorithm>
#include <stdexcept>

SymTable::SymTable(SymTable* prev)
	: prev(prev)
{}

void SymTable::insert(Symbol s)
{
	if (exists(s.getName()))
		throw std::runtime_error("symbol already exists");

	table.insert({s.getName(), s});
}

Symbol SymTable::lookup(const std::string& name) const
{
	return table.at(name);
}

bool SymTable::exists(const std::string& name) const
{
	return table.contains(name);
}
