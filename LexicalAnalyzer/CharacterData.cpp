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

int CharacterData::IS(CharacterClass chClass) {
	switch (chClass) {
	case LETTER:
		return (int)(isalpha(this->data));
	case DIGIT:
		return (int)(isdigit(this->data));
	}

	return 0;
}