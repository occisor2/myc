#include "Compiler.h"
#include "Parse/Parser.h"
#include "Lex/Scanner.h"
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

void assemble(const fs::path& filePath)
{
	std::system(std::format("as -msyntax=intel -mnaked-reg -o out.o {}", filePath.string()).c_str());
	//std::system(std::format("nasm -felf64 {}", filePath.string()).c_str());
}

void link(const fs::path& filePath)
{
	std::system(std::format("cc {}", filePath.string()).c_str());
}

int main(int argc, char* argv[])
{
	Compiler c(argc, argv);
	c.compile();
	
	return EXIT_SUCCESS;
}
