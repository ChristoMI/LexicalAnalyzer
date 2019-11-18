#pragma once

#include <utility>
#include <string>

#define SIGN_DELIMETER_CHARACTER 40
#define COMMENTARY_START_CHARACTER 50


#define EMPTY_LEXEM 7

typedef std::pair<int, int> Position;

typedef char Character;

typedef std::string string;

typedef int CharacterClass;

class Lexem
{
private:
	string data;

	Position position;
public:
	Lexem();
	Lexem(string);
	~Lexem() {};

	Position getStartingPosition();
	void setStartingPosition(int, int);
	void setStartingPosition(Position);

	string getData();
	void setData(string);

	Lexem& operator +=(Character&);

	bool contains(CharacterClass);
};

