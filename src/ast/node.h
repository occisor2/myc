#pragma once

#include "token.h"
#include <string_view>

class Node
{
public:
	Node() = delete;
	Node(Token token);
	virtual void print(std::string_view sourceCode);

private:
	Token token;
};
