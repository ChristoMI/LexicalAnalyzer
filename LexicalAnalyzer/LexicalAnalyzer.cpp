#include "pch.h"
#include "LexicalAnalyzer.h"

CharacterData* LexicalAnalyzer::findChar(const char& currentChar, const AttributesTable& attributes) {
	auto searchResult = attributes.find(currentChar);
	CharacterData* resultData = new CharacterData(currentChar);
	
	if (searchResult != attributes.end()) {
		resultData->verify();
		resultData->setCategory(searchResult->second);
	}

	return resultData;
}

void LexicalAnalyzer::addLog(LexemClass currentClass, Position currentPosition) {
	if (currentClass == INVALID_CHARACTER)
		logMessage += ERROR_INVALID_CHARACTER(currentPosition.first, currentPosition.second);
}


LexerState LexicalAnalyzer::encodeLexem(InformationTable* infoTables, Lexem& lexem, LexemString& lexemVector, LexerState lexerState) {
	auto searchResult = infoTables[KEYWORDS].find(lexem.getData());

	switch (lexerState) {
	case IDENTIFIER:
		if (searchResult != infoTables[KEYWORDS].end()) {
			lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

			return NO_LEXEM_FOUND;
		}
		else if ((searchResult = infoTables[IDENTIFIERS].find(lexem.getData())) != infoTables[IDENTIFIERS].end()) {
			lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

			return NO_LEXEM_FOUND;
		}
		else {
			Token code = infoTables[IDENTIFIERS].size() + 1;
			infoTables[IDENTIFIERS].insert({ lexem.getData(), code });
			lexemVector.push_back(EncodedLexem(lexem, code));

			return NO_LEXEM_FOUND;
		}
	}

	return 0;
}

void LexicalAnalyzer::process(std::string programPath, InformationTable* informationTables, AttributesTable attributesTable)
{
	std::ifstream programFile(programPath);
	Character currentChar;
	LexemClass lexerState = NO_LEXEM_FOUND;

	Lexem currentLexem;
	LexemString lexemVector;

	int currentRow = 1, currentColumn = 1;

	while (programFile.get(currentChar)) {
		if (currentChar == '\n') {
			currentRow++;
			currentColumn = 1;
		}

		CharacterData* currentCharacterData = findChar(currentChar, attributesTable);

		/*if (currentCharacterData->getState() == CHARACTER_NOT_FOUND)
			currentCharacterData->setCategory(INVALID_CHARACTER);*/

		if (lexerState == ERROR)
			break;

		if (currentCharacterData->corresponds(INVALID_CHARACTER)) {
			lexerState = ERROR;

			addLog(ERROR, Position(currentRow, currentColumn));
		}
		else {
			switch (lexerState) {
			case NO_LEXEM_FOUND:
				currentLexem.setStartingPosition(currentRow, currentColumn);

				if (currentCharacterData->corresponds(CONSTANT_CHARACTER)) {
					currentLexem += currentCharacterData->getData();
					lexerState = CONSTANT;
				}
				else if (currentCharacterData->corresponds(IDENTIFIER_CHARACTER)) {
					currentLexem += currentCharacterData->getData();
					lexerState = IDENTIFIER;
				}
				/*else if (currentCharacterData->corresponds(COMPLEX_DELIMETER_CHARACTER)) {
					currentLexem += currentCharacterData->getData();
					lexerState = COMPLEX_DELIMETER;
				}
				else if (currentCharacterData->corresponds(SIMPLE_DELIMETER_CHARACTER))
					lexerState = encodeLexem(currentCharacterData, informationTables, lexemVector, SIMPLE_DELIMETER);*/
				break;
			case IDENTIFIER:
				if (currentCharacterData->IS(LETTER) || currentCharacterData->IS(DIGIT)) {
					currentLexem += currentCharacterData->getData();
					break;
				}
				else {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
				}
			case CONSTANT:
				if (currentCharacterData->IS(DIGIT)) {
					currentLexem += currentCharacterData->getData();
					break;
				}
				else {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
				}
				
				/*else if (currentCharacterData->corresponds(SIMPLE_DELIMETER_CHARACTER)) {
					lexerState = encodeLexem(currentCharacterData, informationTables, lexemVector, lexerState);

					currentCharacterData->setStartingPosition(currentRow, currentColumn);
				}*/
			}
		}

		currentColumn++;
	}
}
