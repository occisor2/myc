#include "Compiler.h"
#include "Lex/Scanner.h"
#include "Parse/Parser.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>

Compiler::Compiler(int argc, char* argv[])
	: options(parseCmdArgs(argc, argv))
{}

void Compiler::compile()
{
	std::ifstream codeFile(options.inputFileName);
	
	Scanner s(codeFile);
	Parser p(s);

	p.parse();
}

Compiler::Options Compiler::parseCmdArgs(int argc, char* argv[])
{
	constexpr auto usageMsg = "[-c asmFileName] [-o outputName] inputName";
	Options options;
	int opt;

	while ((opt = getopt(argc, argv, "c:o:")) != -1)
	{
		switch (opt)
		{
		case 'c':
			options.asmFileName = optarg;
			options.outputAsm = true;
			break;
		case 'o':
			options.outFileName = optarg;
			break;
		default:
			std::cout << "Usage: " << argv[0] << " " << usageMsg << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}

	// Parse the inputFileName at the end.
	if (optind >= argc)
	{
		std::cout << "Usage: " << argv[0] << " " << usageMsg << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	auto inputFileName = argv[optind];
	options.inputFileName = inputFileName;

	// If some options were not provided, set the defaults.
	if (options.outFileName.empty())
	{
		// Make the outFileName the same as input but without extension
		auto outFileName = options.inputFileName.parent_path() / options.inputFileName.stem();
		options.outFileName = outFileName;
	}
	
	return options;
}

void Compiler::printOptions()
{
	std::cout << "Options:" << std::endl;
	std::cout << "Input File: " << options.inputFileName << std::endl;
	std::cout << "Output File: " << options.outFileName << std::endl;
	std::cout << "Asm File: " << options.asmFileName << std::endl;
	std::cout << "Generate Assembly: " << options.outAsm() << std::endl;
}
