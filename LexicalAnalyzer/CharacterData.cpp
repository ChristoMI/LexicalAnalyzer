#include "pch.h"
#include "CharacterData.h"


CharacterData::CharacterData()
{
	this->data = ' ';
	this->state = CHARACTER_NOT_FOUND;
}

CharacterData::CharacterData(Character character) {
	this->data = character;
	this->state = CHARACTER_NOT_FOUND;
}

CharacterData::~CharacterData()
{
}

void CharacterData::verify() {
	this->state = CHARACTER_FOUND;
}

void CharacterData::setCategory(LexemCode newCategory) {
	this->category = newCategory;
}

Character& CharacterData::getData() {
	return this->data;
}

CharacterState CharacterData::getState() {
	return this->state;
}

int CharacterData::corresponds(LexemCode category) {
	return (int)(this->category == category);
}

int CharacterData::IS(CharacterClass chClass, AttributesTable attributes) {
	switch (chClass) {
	case LETTER:
		return (int)(isalpha(this->data));
	case DIGIT:
		return (int)(isdigit(this->data));
	case SIGN_DELIMETER_CHARACTER:
		if (this->data == '<' || this->data == '=' || this->data == '>')
			return 1;

		return 0;
	case WHITESPACE_CHARACTER:
		for (AttributesTable::const_iterator it = attributes.begin(); it != attributes.end(); it++) {
			if (this->data == it->first)
				return 1;
		}

		return 0;
	}
}