#pragma once

#include <utility>

#include <stdio.h>
#include <ctype.h>

#include "TablesInfo.h"

// Categories:
#define WHITESPACE_CHARACTER 0
#define CONSTANT_CHARACTER 1
#define IDENTIFIER_CHARACTER 2

//#define SIMPLE_DELIMETER 4
#define SIGN_DELIMETER_CHARACTER 40

#define LESS_OR_EQUAL 41
#define MORE_OR_EQUAL 42
#define NOT_EQUAL 43

#define INVALID_CHARACTER 6

#define LETTER 6
#define DIGIT 7

//States:
#define CHARACTER_NOT_FOUND 0 
#define CHARACTER_FOUND 1

typedef char Character;

typedef int CharacterClass;
typedef int CharacterState;

typedef int Code;

typedef int LexemCode;

typedef std::pair<int, int> Position;

class CharacterData
{
private:
	Character data;

	CharacterState state;

	LexemCode category;
public:
	CharacterData();

	CharacterData(Character);

	~CharacterData();

	Character& getData();

	CharacterState getState();

	void setCategory(LexemCode);

	void verify();

	int corresponds(LexemCode);

	int IS(CharacterClass, AttributesTable attributes = AttributesTable());
};

