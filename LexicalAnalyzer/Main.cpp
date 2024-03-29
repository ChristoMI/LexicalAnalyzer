﻿#include "pch.h"
#include <iostream>

#include "LexicalAnalyzer.h"

#define CASE1 "Tests/Case1.txt"
#define CASE2 "Tests/Case2.txt"
#define CASE3 "Tests/Case3_Delimeters.txt"
#define CASE4 "Tests/Case4_SimpleDelimeters.txt"
#define CASE5 "Tests/Case5_WrongCommentary.txt"
#define CASE5 "Tests/Case5_WrongCommentary.txt"
#define CASE6 "Tests/Case6_Commentary.txt"
#define CASE7 "Tests/Case7_WildCommentary.txt"
#define CASE8 "Tests/Case8_UnclosedCommentary.txt"
#define CASE9 "Tests/Case9_NoCommentaryStart.txt"
#define CASE10 "Tests/Case10.txt"

InformationTable informationTables[TABLES_QUANTITY];
AttributesTable attributes;

LexicalAnalyzer* lexer = new LexicalAnalyzer();

void initTables();
void display(LexicalAnalyzer*);

int main() {
	initTables();
	
	lexer->process(CASE10, &informationTables[0], attributes);

	display(lexer);

	return 0;
}

void initTables() {
	informationTables[KEYWORDS] = readTable<InformationTable, string>(KEYWORDS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[IDENTIFIERS] = readTable<InformationTable, string>(IDENTIFIERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[CONSTANTS] = readTable<InformationTable, string>(CONSTANTS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[SIMPLE_DELIMETERS] = readTable<InformationTable, string>(SIMPLE_DELIMETERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[COMPLEX_DELIMETERS] = readTable<InformationTable, string>(COMPLEX_DELIMETERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });

	attributes = readTable<AttributesTable, char>(ATTRIBUTES_PATH, [](std::string parsingIdentifier) -> char {return char(atoi(parsingIdentifier.c_str())); });

	int k = 0;
}

void display(LexicalAnalyzer* lexer) {
	if (lexer->getState() == ERROR)
		std::cout << lexer->getLog();
	else {
		for (auto &lexem : lexer->getLexemVector()) 
			std::cout << lexem.toString() << std::endl;
	}
}