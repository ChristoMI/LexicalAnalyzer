#pragma once

#include <utility>

#include <stdio.h>
#include <ctype.h>

// Categories:
#define WHITESPACE_CHARACTER 0
#define CONSTANT_CHARACTER 1
#define IDENTIFIER_CHARACTER 2
#define SIMPLE_DELIMETER_CHARACTER 3
#define COMPLEX_DELIMETER_CHARACTER 4
#define COMMENTARY_CHARACTER 4
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

	int IS(CharacterClass);
};

