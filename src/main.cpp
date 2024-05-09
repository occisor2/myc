#include "analysis/nameresolution.h"
#include "analysis/printast.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include <getopt.h>
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

static constexpr auto helpMsg = "Usage: myc [options] file...";

int main(int argc, char* argv[])
{
	// parse command line arguments
	bool dumpTokens = false;
	bool dumpAST = false;
	std::string filePath;

	int opt;
	while ((opt = getopt(argc, argv, "-ta")) != -1)
	{
		switch (opt)
		{
		case 't':
			dumpTokens = true;
			break;
		case 'a':
			dumpAST = true;
			break;
		case 1:
			filePath = optarg;
			break;
		default:
			std::cout << helpMsg << std::endl;
			return EXIT_FAILURE;
		}
	}

	// make sure file path was supplied
	if (filePath.empty())
	{
		std::cout << helpMsg << std::endl;
		return EXIT_FAILURE;
	}

	auto sourceCode = readFile(filePath);

	if (dumpTokens)
	{
		std::cout << "Tokens:" << std::endl;
		Scanner s(sourceCode);
		for (Token t = s.scan(); not s.isEmpty(); t = s.scan())
		{
			// if (t.type == Token::Type::Int)
			// 	std::cout << "yes" << std::endl;
			std::cout << sourceCode.substr(t.pos, t.size) << std::endl;
		}
	}
	if (dumpAST)
	{
		Parser p(sourceCode, filePath);
		auto func = p.parse();
		if (not func)
			return EXIT_FAILURE;

		NameResolution res(filePath, sourceCode);
		res.visit(*func);

		PrintAST print;
		print.visit(*func);
	}

	return EXIT_SUCCESS;
}
