#include "Generator.h"
#include "Parser.h"
#include "Scanner.h"
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

void assemble(const fs::path& filePath)
{
	std::system(std::format("as -msyntax=intel -mnaked-reg {}", filePath.string()).c_str());
	//std::system(std::format("nasm -felf64 {}", filePath.string()).c_str());
}

void link(const fs::path& filePath)
{
	std::system(std::format("cc {}", filePath.string()).c_str());
}

void compile(const fs::path& filePath)
{
	std::ifstream codeFile(filePath);
	Scanner scanner(codeFile);

	try
	{
		std::cout << "Tokens:" << std::endl;
		// Break up code into tokens
		auto tokens =  scanner.scan();
		for (auto& t : tokens)
			std::cout << t << std::endl;

		std::cout << std::endl;
		// Parse tokens into an AST
		Parser parser(tokens);
		auto ast = parser.parse();
		// Generate the assembly code.
		Generator gen(ast);

		std::ofstream os("./out.s");
		gen.generate(os);
		os.close();
		// Assemble and link
		assemble("./out.s");
		link("./out.o");
	}
	catch (ScanError& e)
	{
		std::cerr << "Error, line " << e.line << ": " << e.what() << std::endl;
	}
	catch (ParseError& p)
	{
		std::cerr << "Error, line " << p.line << ": " << p.what() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " [path to code file]" << std::endl;
		return EXIT_FAILURE;
	}

	compile(argv[1]);
	
	return EXIT_SUCCESS;
}
