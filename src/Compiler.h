#pragma once

#include <filesystem>

class Compiler
{	
	class Options
	{
	public:
		std::filesystem::path inputFileName;
		std::filesystem::path outFileName;
		std::filesystem::path asmFileName;
		bool outputAsm = false;
		
		Options() = default;
		inline bool outAsm() { return outputAsm; }
	};
	
public:
	Compiler(int argc, char* argv[]);

	void compile();
	void printOptions();
	
private:
	Options options;
	
	Options parseCmdArgs(int argc, char* argv[]);
	std::filesystem::path assemble(std::filesystem::path asmFile);
	void link(std::filesystem::path objFile);
};
