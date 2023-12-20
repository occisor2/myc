#pragma once

#include <cstddef>
#include <string>

class Symbol
{
public:
	enum class Type
	{
		Local,
		Temp,
		Function
	};

	size_t stackOffset = 0;
	
	Symbol() = default;
	Symbol(std::string name, Type type);

	std::string getName() const;
	Type getType() const;

private:
	std::string name;
	Type type{};
};
