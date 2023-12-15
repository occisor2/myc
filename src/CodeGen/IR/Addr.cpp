#include "Addr.h"
#include <string>

using namespace IR;

// Default setup to where it won't be printed on debug if empty
Addr::Addr()
	: name(""), type(Type::Name), temp(false)
{}

Addr::Addr(int constant)
	: constant(constant), type(Type::Constant)
{}

Addr::Addr(std::string name, bool temp)
	: name(name), type(Type::Name), temp(temp)
{}
	
Addr::Type Addr::getType() const
{
	return type;
}

bool Addr::isTemp() const
{
	return temp;
}

std::ostream& IR::operator<<(std::ostream& os, const IR::Addr& t)
{
	switch (t.getType())
	{
	case Addr::Type::Constant:
		os << t.constant;
		break;
	case Addr::Type::Name:
		if (t.isTemp())
			os << "t";
		os << t.name;
		break;
	}

	return os;
}
