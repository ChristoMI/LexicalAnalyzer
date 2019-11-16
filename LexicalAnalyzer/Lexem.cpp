#include "pch.h"
#include "Lexem.h"

Lexem::Lexem()
{
	this->data = "";
	this->position.first = -1;
	this->position.second = -1;
}

Lexem::Lexem(string data)
{
	this->data = data;
	this->position.first = -1;
	this->position.second = -1;
}

Position Lexem::getStartingPosition()
{
	return this->position;
}

void Lexem::setStartingPosition(int row, int column) {
	this->position.first = row;
	this->position.second = column;
}
void Lexem::setStartingPosition(Position newPosition) {
	this->position = newPosition;
}

string Lexem::getData()
{
	return this->data;
}

void Lexem::setData(string newData)
{
	this->data = newData;
}

Lexem& Lexem::operator +=(Character& currentChar) {
	this->data += currentChar;

	return *this;
}

bool Lexem::contains(CharacterClass characterClass)
{
	switch (characterClass) {
	case SIGN_DELIMETER_CHARACTER:
		if (this->data == ">" || this->data == "<" || this->data == "=")
			return true;
		
		return false;
	case PARENTHESIS_CHARACTER:
		if (this->data == "(")
			return true;

		return false;
	}
}
