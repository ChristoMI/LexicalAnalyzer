#pragma once
#include "Lexem.h"

typedef int LexemCode;

class EncodedLexem :
	public Lexem
{
private:
	LexemCode code;
public:
	EncodedLexem() {};
	EncodedLexem(Lexem&, LexemCode&);
	~EncodedLexem() {};

	void setCategory(LexemCode);
};

