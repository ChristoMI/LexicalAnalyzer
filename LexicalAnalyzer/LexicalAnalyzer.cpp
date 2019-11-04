#include "pch.h"
#include "LexicalAnalyzer.h"


LexicalAnalyzer::LexicalAnalyzer()
{
}


LexicalAnalyzer::~LexicalAnalyzer()
{
}

void LexicalAnalyzer::process(std::string programPath)
{
	std::ifstream programFile(programPath);
	char currentSymbol;


	while (programFile.get(currentSymbol)) {
		std::cout << currentSymbol;
	}
}
