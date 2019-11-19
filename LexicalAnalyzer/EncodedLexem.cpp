#include "pch.h"
#include "EncodedLexem.h"


EncodedLexem::EncodedLexem(Lexem & lexem, LexemCode & category)
{
	this->setData(lexem.getData());
	this->setStartingPosition(lexem.getStartingPosition());
	this->setCategory(category);
}

void EncodedLexem::setCategory(LexemCode newCategory)
{
	this->code = newCategory;
}

string EncodedLexem::toString() {
	return std::to_string(this->position.first) + " " + std::to_string(this->position.second) + " " + std::to_string(this->code) + " " + this->data;
}