#include "X86.h"
#include "CodeGen/IR/Instruct.h"
#include "Symbol/SymTable.h"
#include "Symbol/Symbol.h"
#include <format>
#include <iostream>
#include <utility>

X86::X86(const std::vector<IR::Instruct>& IRCode, SymTable& symTable)
	: IRCode(IRCode), symTable(symTable)
{}

void X86::gen()
{
	calculateLocalSize();
	genPrologue();
	generateCode();
	genEpilogue();
}

std::string X86::getCode()
{
	return assembly.str();
}

void X86::calculateLocalSize()
{
	// Treats temporaries as locals for now
	for (auto& [name, symbol] : symTable.getSymbols())
	{
		//std::cout << "Symbol: " << name << '\n';
		
		localsTotalSize += 4;
		symbol.stackOffset = localsTotalSize;
	}
}

void X86::generateCode()
{
	for (auto& instruct : IRCode)
	{	
		switch (instruct.op)
		{
		case IR::Instruct::Operator::Add:
			add(instruct);
			break;
		case IR::Instruct::Operator::Sub:
			sub(instruct);
			break;
		case IR::Instruct::Operator::Mul:
			mul(instruct);
			break;
		case IR::Instruct::Operator::Div:
			div(instruct);
			break;
		case IR::Instruct::Operator::Assign:
			assign(instruct);
			break;
		}
	}
}

void X86::genPrologue()
{
	// Setup start of file
	assembly << ".globl main" << "\n\n";
	assembly << ".text" << "\n";
	// Name the main function
	assembly << "main:" << "\n";
	// Setup stack frame
	inst("push rbp");
	inst("mov rbp, rsp");
	assembly << "\n";
	
	// Allocate locals
	inst(std::format("sub rsp, {}", localsTotalSize));
}

void X86::genEpilogue()
{
	// Destroy locals and stack frame
	inst("leave");
	// Return success
	inst("mov rax, 0");
	inst("ret");
}

void X86::add(const IR::Instruct& instruct)
{
	loadReg("eax", instruct.arg1.name);
	loadReg("ecx", instruct.arg2.name);
	inst("add eax, ecx");
	saveReg("eax", instruct.result.name);
}

void X86::sub(const IR::Instruct& instruct)
{
	loadReg("eax", instruct.arg1.name);
	loadReg("ecx", instruct.arg2.name);
	inst("sub eax, ecx");
	saveReg("eax", instruct.result.name);	
}

void X86::mul(const IR::Instruct& instruct)
{
	loadReg("eax", instruct.arg1.name);
	loadReg("ecx", instruct.arg2.name);
	inst("add eax, ecx");
	saveReg("eax", instruct.result.name);
}

void X86::div(const IR::Instruct& instruct)
{
	loadReg("eax", instruct.arg1.name);
	loadReg("ecx", instruct.arg1.name);
	inst("idiv ecx");
	saveReg("eax", instruct.result.name);
}

void X86::assign(const IR::Instruct& instruct)
{
	
}

void X86::loadReg(const std::string& reg, const std::string& sym)
{
	auto offset = symTable.lookup(sym).stackOffset;
	inst(std::format("mov {}, DWORD PTR [rbp-{}]", reg, offset));
}

void X86::saveReg(const std::string& reg, const std::string& sym)
{
	auto offset = symTable.lookup(sym).stackOffset;
	inst(std::format("mov {}, DWORD PTR [rbp-{}]", reg, offset));
}

void X86::inst(const std::string& as)
{
	assembly << as << '\n';
}
