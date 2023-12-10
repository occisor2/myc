#include "RegisterAllocator.h"
#include <stdexcept>

RegisterAllocator::RegisterAllocator()
{
	freeAll();
}

void RegisterAllocator::freeAll()
{
	freeList.fill(true);
}

void RegisterAllocator::free(reg_t reg)
{
	if (freeList[reg])
		throw std::runtime_error("trying to free already freed register");
	freeList[reg] = true;
}

reg_t RegisterAllocator::allocate()
{
	for (reg_t i = 0; i < freeList.size(); ++i)
		if (freeList[i])
		{
			freeList[i] = false;
			return i;
		}

	throw std::runtime_error("out of registers");
}

const char* RegisterAllocator::getReg(reg_t reg)
{
	return registers[reg];
}
