#pragma once

#include "Symbol/Symbol.h"
#include <format>
#include <ostream>
#include <string>

namespace IR
{

class Addr
{
public:
	enum class Type
	{
		Constant,
		Name
	};

	std::string name;
	int constant;

	Addr() = default;
	explicit Addr(int constant);
	Addr(std::string name, bool temp = false);
	
	Type getType() const;
	bool isTemp() const;

	friend std::ostream& operator<<(std::ostream& os, const Addr& t);

private:
	Type type{};
	bool temp = false;
};

std::ostream& operator<<(std::ostream& os, const Addr& t);

}
