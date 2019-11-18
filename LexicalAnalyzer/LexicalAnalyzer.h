#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "CharacterData.h"
#include "TablesInfo.h"
#include "EncodedLexem.h"

// Logs:
#define ERROR_INVALID_CHARACTER(x,y) ("Invalid symbol at position: " + std::to_string(x) + ", " + std::to_string(y) + "\n")
#define ERROR_INVALID_OPERATOR(x,y) ("Invalid operator at position: " + std::to_string(x) + ", " + std::to_string(y) + "\n")
#define ERROR_UNCLOSED_COMMENTARY(x,y) ("Unclosed commentary at position: " + std::to_string(x) + ", " + std::to_string(y) + "\n")
// LexerState:
#define UNCLOSED_COMMENTARY -3
#define INVALID_OPERATOR -2
#define ERROR -1
#define NO_LEXEM_FOUND 0 


// LexemState:
#define CONSTANT 1
#define IDENTIFIER 2
#define DELIMETER 3
#define COMPLEX_DELIMETER 4
#define COMMENTARY 5

#define COMMENTARY_START 501
#define COMMENTARY_END 502

// Positioning:
#define IDENTIFIERS_OFFSET 400

typedef std::vector<EncodedLexem> LexemString;

typedef int LexemClass;

typedef int LexerState;

class LexicalAnalyzer
{
private:
	CharacterData* findChar(const char&, const AttributesTable&, int);

	void addLog(LexemClass, Position);

	std::string logMessage;

	LexerState encodeLexem(InformationTable*, Lexem&, LexemString&, LexerState);

	LexemString lexemVector;

	LexemClass lexerState = NO_LEXEM_FOUND;
public:
	LexicalAnalyzer() {};
	~LexicalAnalyzer() {};

	void process(std::string, InformationTable*, AttributesTable);

	bool IS(std::string, CharacterClass);
};

