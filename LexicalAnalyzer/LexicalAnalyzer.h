#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	void process(std::string);
};

