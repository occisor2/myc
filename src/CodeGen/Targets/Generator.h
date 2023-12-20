#pragma once

#include "CodeGen/IR/Instruct.h"
#include "Symbol/SymTable.h"
#include <string>
#include <vector>

/**
 * Class for generating machine specific assembly. 
 */
class Generator
{
public:
	Generator(SymTable& symTable);
};
