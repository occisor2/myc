#include "SymTable.h"
#include "Symbol.h"
#include <algorithm>
#include <stdexcept>

SymTable::SymTable(SymTable* prev)
	: prev(prev)
{}

std::unordered_map<std::string, Symbol>& SymTable::getSymbols()
{
	return table;
}

void SymTable::insert(Symbol s)
{
	if (exists(s.getName()))
		throw std::runtime_error("symbol already exists");

	table.insert({s.getName(), s});
}

Symbol SymTable::lookup(const std::string& name) const
{
	auto head = this;
	
	while (head)
	{
		if (head->table.contains(name))
			return head->table.at(name);
		head = head->prev;
	}
	
	throw std::runtime_error("symbol does not exist");
}

bool SymTable::exists(const std::string& name) const
{
	auto head = this;
	
	while (head)
	{
		if (head->table.contains(name))
			return true;
		head = head->prev;
	}
	
	return false;
}
