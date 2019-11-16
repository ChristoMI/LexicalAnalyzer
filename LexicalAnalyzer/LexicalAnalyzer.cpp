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
	else if(currentClass == INVALID_OPERATOR)
		logMessage += ERROR_INVALID_OPERATOR(currentPosition.first, currentPosition.second);
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
	case CONSTANT:
		if (searchResult != infoTables[CONSTANTS].end()) {
			lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

			return NO_LEXEM_FOUND;
		}
		else {
			Token code = infoTables[CONSTANTS].size() + 1;
			infoTables[CONSTANTS].insert({ lexem.getData(), code });
			lexemVector.push_back(EncodedLexem(lexem, code));

			return NO_LEXEM_FOUND;
		}
	case DELIMETER:
		searchResult = infoTables[SIMPLE_DELIMETERS].find(lexem.getData());
		lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

		return NO_LEXEM_FOUND;
	case COMPLEX_DELIMETER:
		searchResult = infoTables[COMPLEX_DELIMETERS].find(lexem.getData());
		lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

		return NO_LEXEM_FOUND;
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
				else if (currentCharacterData->corresponds(DELIMETER)) {
					currentLexem += currentCharacterData->getData();
					lexerState = DELIMETER;
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
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");

					break;
				}
			case CONSTANT:
				if (currentCharacterData->IS(DIGIT)) {
					currentLexem += currentCharacterData->getData();
					break;
				}
				else {
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");

					break;
				}
			case DELIMETER:
				if (currentCharacterData->IS(SIGN_DELIMETER_CHARACTER)) {
					if (currentLexem.contains(SIGN_DELIMETER_CHARACTER)) {
						if (IS(currentLexem.getData() + currentCharacterData->getData(), LESS_OR_EQUAL) || 
							IS(currentLexem.getData() + currentCharacterData->getData(), MORE_OR_EQUAL) ||
							IS(currentLexem.getData() + currentCharacterData->getData(), NOT_EQUAL)) {

							currentLexem += currentCharacterData->getData();

							lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState = COMPLEX_DELIMETER);
							currentLexem.setData("");

							break;
						}
						else {
							lexerState = ERROR;

							addLog(INVALID_OPERATOR, Position(currentRow, currentColumn));

							break;
						}
					}
				}
				else if (currentCharacterData->IS(WHITESPACE_CHARACTER, attributesTable)) {
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");

					break;
				}
				else if (currentCharacterData->IS(LETTER)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					currentLexem.setData(std::to_string(currentCharacterData->getData()));
					lexerState = IDENTIFIER;

					break;
				}
				else if (currentCharacterData->IS(DIGIT)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					currentLexem.setData(std::to_string(currentCharacterData->getData()));
					lexerState = CONSTANT;

					break;
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

bool LexicalAnalyzer::IS(std::string lexem, CharacterClass characterClass)
{
	switch (characterClass) {
	case LESS_OR_EQUAL:
		if (lexem == "<=")
			return true;

		return false;
	case MORE_OR_EQUAL:
		if (lexem == ">=")
			return true;

		return false;
	case NOT_EQUAL:
		if (lexem == "<>")
			return true;

		return false;
	}
}
