#pragma once

#include <iostream>

class Token
{
public:
	enum class Type
	{
		Plus,
		Minus,
		Star,
		Slash,
		IntLit
	};

	Token() = default;
	explicit Token(Type type);
	Token(Type type, int intLit);

	friend std::ostream& operator<<(std::ostream& os, const Token& t);
	
	Type type{};
	int intLit{};
	unsigned int line{};
};
