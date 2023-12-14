#pragma once

#include <string>

class Symbol
{
public:
	Symbol() = default;
	Symbol(std::string name);

	std::string getName() const;

private:
	std::string name;
};
