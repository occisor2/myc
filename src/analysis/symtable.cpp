#include "symtable.h"

Symbol::Symbol()
{}

Symbol::Symbol(std::string name)
	: name(name)
{}

SymTable::SymTable(SymTable* prev)
	: prev(prev)
{}

bool SymTable::lookup(std::string name)
{
	if (table.contains(name))
		return true;

	if (prev)
		return prev->lookup(name);
	else
		return false;
}

void SymTable::insert(Symbol symbol)
{
	table[symbol.name] = symbol;
}
