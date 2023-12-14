#include "Symbol.h"

Symbol::Symbol(std::string name)
	: name(name)
{}

std::string Symbol::getName() const
{
	return name;
}
