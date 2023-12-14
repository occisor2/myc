#pragma once

#include <iostream>
#include <string>

class Token
{
public:
	enum class Type
	{
		Eof,
		Plus,
		Minus,
		Star,
		Slash,
		IntLit,
		Semi,
		Ident
	};

	Token() = default;
	explicit Token(Type type);
	Token(Type type, int intLit);

	friend std::ostream& operator<<(std::ostream& os, const Token& t);
	
	Type type{};
	int intLit{};
	std::string ident;
};
