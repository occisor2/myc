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
		Ident,
		Int,
		Assign,
		OpenBrace,
		CloseBrace,
		OpenParen,
		CloseParen,
		Not,
		Equal,
		NotEqual,
		Less,
		Greater,
		LessEqual,
		GreaterEqual,
		If
	};

	explicit Token(Type type);
	// Normal identifiers
	explicit Token(std::string ident);
	// Keywords and others
	Token(Type type, std::string ident);
	// Integers
	Token(int intLit, std::string numStr);

	Type getType() const;
	int getIntLit() const;
	std::string getText() const;
	std::string getIdent() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Token& t);

private:
	Type type{};
	int intLit{};
	// Used to hold name of an identifier or the text representing a
	// token
	std::string ident;
};
