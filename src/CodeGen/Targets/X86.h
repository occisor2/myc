#pragma once

#include "CodeGen/IR/Instruct.h"
#include "Generator.h"
#include "Symbol/SymTable.h"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

class X86
{
public:
	X86(const std::vector<IR::Instruct>& IRCode, SymTable& symTable);

	void gen();
	std::string getCode();

private:
	const std::vector<IR::Instruct>& IRCode;
	SymTable& symTable;
	std::stringstream assembly;
	size_t localsTotalSize = 0;

	void calculateLocalSize();
	void generateCode();
	void genPrologue();
	void genEpilogue();

	void add(const IR::Instruct& instruct);
	void sub(const IR::Instruct& instruct);
	void mul(const IR::Instruct& instruct);
	void div(const IR::Instruct& instruct);
	void assign(const IR::Instruct& instruct);

	void loadReg(const std::string& reg, const std::string& sym);
	void saveReg(const std::string& reg, const std::string& sym);
	void inst(const std::string& as);
};
