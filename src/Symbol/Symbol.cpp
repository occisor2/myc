#include "Symbol.h"

Symbol::Symbol(std::string name, Type type)
	: name(name), type(type)
{}

std::string Symbol::getName() const
{
	return name;
}

Symbol::Type Symbol::getType() const
{
	return type;
}
