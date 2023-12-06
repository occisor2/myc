#include "Parser.h"
#include "Scanner.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " [path to code file]" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream codeFile(argv[1]);
	Scanner scanner(codeFile);

	try
	{
		auto tokens =  scanner.scan();
		for (auto& t : tokens)
			std::cout << t << std::endl;

		Parser parser(tokens);
		parser.parse();
	}
	catch (ScanError& e)
	{
		std::cerr << "Error, line " << e.line << ": " << e.what() << std::endl;
	}
	catch (ParseError& p)
	{
		std::cerr << "Error, line " << p.line << ": " << p.what() << std::endl;
	}
	
	return EXIT_SUCCESS;
}
