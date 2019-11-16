#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "CharacterData.h"
#include "TablesInfo.h"
#include "EncodedLexem.h"

// Logs:
#define ERROR_INVALID_CHARACTER(x,y) ("Invalid symbol at position" + std::to_string(x) + ", " + std::to_string(y) + "\n")
#define ERROR_INVALID_OPERATOR(x,y) ("Invalid operator at position" + std::to_string(x) + ", " + std::to_string(y) + "\n")

// LexerState:
#define INVALID_OPERATOR -2
#define ERROR -1
#define NO_LEXEM_FOUND 0 


// LexemState:
#define CONSTANT 1
#define IDENTIFIER 2
#define DELIMETER 3
#define COMPLEX_DELIMETER 4
#define COMMENTARY 5

// Positioning:
#define IDENTIFIERS_OFFSET 400

typedef std::vector<EncodedLexem> LexemString;

typedef int LexemClass;

typedef int LexerState;

class LexicalAnalyzer
{
private:
	CharacterData* findChar(const char&, const AttributesTable&);

	void addLog(LexemClass, Position);

	std::string logMessage;

	LexerState encodeLexem(InformationTable*, Lexem&, LexemString&, LexerState);
public:
	LexicalAnalyzer() {};
	~LexicalAnalyzer() {};

	void process(std::string, InformationTable*, AttributesTable);

	bool IS(std::string, CharacterClass);
};

