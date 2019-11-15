#pragma once

#include <utility>
#include <string>

typedef std::pair<int, int> Position;

typedef char Character;

typedef std::string string;

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
};

