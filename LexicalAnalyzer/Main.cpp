﻿#include "pch.h"
#include <iostream>

#include "TablesInfo.h"
#include "LexicalAnalyzer.h"

#define FILE_PATH "Tests/Case1.txt"

InformationTable informationTables[TABLES_QUANTITY];
AttributesTable attributes;

LexicalAnalyzer* lexer = new LexicalAnalyzer();

void initTables();

int main() {
	initTables();
	
	lexer->process(FILE_PATH);

	return 0;
}

void initTables() {
	informationTables[KEYWORDS] = readTable<InformationTable, string>(TABLES_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[SIMPLE_DELIMETERS] = readTable<InformationTable, string>(SIMPLE_DELIMETERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[COMPLEX_DELIMETERS] = readTable<InformationTable, string>(COMPLEX_DELIMETERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[IDENTIFIERS] = readTable<InformationTable, string>(IDENTIFIERS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });
	informationTables[CONSTANTS] = readTable<InformationTable, string>(CONSTANTS_PATH, [](std::string parsingIdentifier) -> string {return parsingIdentifier; });

	attributes = readTable<AttributesTable, char>(ATTRIBUTES_PATH, [](std::string parsingIdentifier) -> char {return char(atoi(parsingIdentifier.c_str())); });
}