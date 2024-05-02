#include "parser.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path)
{
	std::ifstream file(path);

	if (not file.is_open())
		std::cerr << "could not open file, " << path << std::endl;

	std::stringstream buff;
	buff << file.rdbuf();

	return buff.str();
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " [file path]" << std::endl;
		return EXIT_FAILURE;
	}

	auto sourceCode = readFile(argv[1]);
	Parser p(sourceCode);
	p.parse();
	
	return EXIT_SUCCESS;
}
