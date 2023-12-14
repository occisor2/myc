#pragma once

#include "Addr.h"
#include <ostream>
#include <string>
#include <unordered_map>

namespace IR
{

class Instruct
{
public:
	enum class Operator
	{
		Add, Sub, Mul, Div
	};

	const std::unordered_map<Operator, std::string> opTextTable = {
		{Operator::Add, "add"},
		{Operator::Sub, "sub"},
		{Operator::Mul, "mul"},
		{Operator::Div, "div"}
	};

	// Binary operator
	Instruct(Operator op, Addr arg1, Addr arg2, Addr result);
	
	Operator op{};
	Addr arg1;
	Addr arg2;
	Addr result;

	friend std::ostream& operator<<(std::ostream& os, const Instruct& t);
};

std::ostream& operator<<(std::ostream& os, const IR::Instruct& t);

}
