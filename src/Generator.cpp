#include "Generator.h"
#include <format>
#include <stdexcept>

Generator::Generator(const AST& ast)
	: ast(ast)
{}
	
void Generator::generate(std::ostream& os)
{
	if (not ast.root)
		return;
	
	preamble();

	auto retReg = genAST(ast.root.get());
	loadRetReg(retReg);

	postamble();
	
	os << ss.str();
}

reg_t Generator::genAST(const AST::Node* node)
{
	reg_t leftReg{}, rightReg{};

	if (node->left) leftReg = genAST(node->left.get());
	if (node->right) rightReg = genAST(node->right.get());

	switch (node->type)
	{
	case AST::Type::Add: return add(leftReg, rightReg);
	case AST::Type::Subtract: return sub(leftReg, rightReg);
	case AST::Type::Multiply: return mul(leftReg, rightReg);
	case AST::Type::Divide: return div(leftReg, rightReg);
	case AST::Type::IntLit: return loadReg(node->intLit);
	default:
		throw std::runtime_error("uknown AST operator");
	}
}

void Generator::inst(const std::string& s)
{
	ss << "\t" << s << "\n";
}

void Generator::preamble()
{
	ss << ".globl main" << "\n\n";
	ss << ".text" << "\n";
	
	ss << "main:" << "\n";
	
	inst("push rbp");
	inst("mov rbp, rsp");
	ss << "\n";
}

void Generator::postamble()
{
	ss << "\n";
	inst("leave");
	inst("ret");
}

void Generator::loadRetReg(reg_t reg)
{
	auto valReg = ra.getReg(reg);
	inst(std::format("mov rax, {}", valReg));
}

reg_t Generator::loadReg(int value)
{
	auto reg = ra.allocate();
	inst(std::format("mov {}, {}", ra.getReg(reg), value));
	return reg;	
}

reg_t Generator::add(int r1, int r2)
{
	inst(std::format("add {}, {}", ra.getReg(r1), ra.getReg(r2)));
	ra.free(r2);
	return r1;
}

reg_t Generator::sub(int r1, int r2)
{
	inst(std::format("sub {}, {}", ra.getReg(r1), ra.getReg(r2)));
	ra.free(r2);
	return r1;
}

reg_t Generator::mul(int r1, int r2)
{
	inst(std::format("imul {}, {}", ra.getReg(r1), ra.getReg(r2)));
	ra.free(r2);
	return r1;	
}

reg_t Generator::div(int r1, int r2)
{
	inst(std::format("mov rax, {}", ra.getReg(r1)));
	inst("cqo");
	inst(std::format("idiv {}", ra.getReg(r2)));
	inst(std::format("mov {}, rax", ra.getReg(r1)));
	ra.free(r2);
	return r1;
}
