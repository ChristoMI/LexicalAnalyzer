#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

#define TABLES_QUANTITY 5

#define KEYWORDS 0
#define SIMPLE_DELIMETERS 1
#define COMPLEX_DELIMETERS 2
#define IDENTIFIERS 3
#define CONSTANTS 4

#define KEYWORDS_PATH "Tables\\keywords.tbl"
#define SIMPLE_DELIMETERS_PATH "Tables\\simplDel.tbl"
#define COMPLEX_DELIMETERS_PATH "Tables\\complexDel.tbl"
#define IDENTIFIERS_PATH "Tables\\identifiers.tbl"
#define CONSTANTS_PATH "Tables\\constants.tbl"
#define ATTRIBUTES_PATH "Tables\\attributes.tbl"

typedef std::string string;
typedef char Char;
typedef int Token;
typedef int Code;

typedef std::unordered_map<string, Token> InformationTable;
typedef std::unordered_map<Char, Code> AttributesTable;

template <typename Table, typename Key>
Table readTable(std::string filename, Key(*parse)(std::string), char delimiter = ' ')
{
	Table table(127);
	std::ifstream inputFile(filename);

	std::string inputString;
	string value;
	Token code;

	while (std::getline(inputFile, inputString))
	{
		int delimiterPosition = inputString.rfind(delimiter);
		value = inputString.substr(0, delimiterPosition);
		code = atoi(inputString.substr(delimiterPosition + 1).c_str());

		table.insert({ parse(value), code });
	}

	inputFile.close();
	return table;
}