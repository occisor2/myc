#pragma once

#include "AST.h"
#include "RegisterAllocator.h"
#include <array>
#include <ostream>
#include <sstream>
#include <string>

class Generator
{	
public:
	Generator(const AST& ast);
	
	void generate(std::ostream& os);

private:
	const AST& ast;
	std::stringstream ss;
	RegisterAllocator ra;

	reg_t genAST(const AST::Node* node);

	void inst(const std::string& s);

	void preamble();
	void postamble();
	void loadRetReg(reg_t reg);

	reg_t loadReg(int value);
	reg_t add(int r1, int r2);
	reg_t sub(int r1, int r2);
	reg_t mul(int r1, int r2);
	reg_t div(int r1, int r2);
};
