#pragma once

#include <array>

using reg_t = std::array<const char*, 4>::size_type;

class RegisterAllocator
{
public:
	RegisterAllocator();

	void freeAll();
	void free(reg_t reg);
	reg_t allocate();
	const char* getReg(reg_t reg);

private:
	static constexpr std::array<const char*, 4> registers = {"r8", "r9", "r10", "r11"};
	std::array<bool, registers.size()> freeList;
};
