﻿#include "pch.h"
#include "LexicalAnalyzer.h"

CharacterData* LexicalAnalyzer::findChar(const char& currentChar, const AttributesTable& attributes, int commentaryFlag = 0) {
	auto searchResult = attributes.find(currentChar);
	CharacterData* resultData = new CharacterData(currentChar);
	
	if (!commentaryFlag && searchResult != attributes.end()) {
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
	else if(currentClass == UNCLOSED_COMMENTARY)
		logMessage += ERROR_UNCLOSED_COMMENTARY(currentPosition.first, currentPosition.second);
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
			Token code = infoTables[IDENTIFIERS].size() + IDENTIFIERS_OFFSET;
			infoTables[IDENTIFIERS].insert({ lexem.getData(), code });
			lexemVector.push_back(EncodedLexem(lexem, code));

			return NO_LEXEM_FOUND;
		}
	case CONSTANT:
		if ((searchResult = infoTables[CONSTANTS].find(lexem.getData())) != infoTables[CONSTANTS].end()) {
			lexemVector.push_back(EncodedLexem(lexem, searchResult->second));

			return NO_LEXEM_FOUND;
		}
		else {
			Token code = infoTables[CONSTANTS].size() + CONSTANTS_OFFSET;
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

	Lexem currentLexem;

	int currentRow = 1, currentColumn = 1;

	while (programFile.get(currentChar)) {
		if (currentChar == '\n') {
			currentRow++;
			currentColumn = 1;
		}

		CharacterData* currentCharacterData = findChar(currentChar, attributesTable, lexerState == COMMENTARY);

		/*if (currentCharacterData->getState() == CHARACTER_NOT_FOUND)
			currentCharacterData->setCategory(INVALID_CHARACTER);*/

		if (lexerState == ERROR)
			break;

		if (currentCharacterData->corresponds(INVALID_CHARACTER)) {
			lexerState = ERROR;

			addLog(INVALID_CHARACTER, Position(currentRow, currentColumn));
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
				else if (currentCharacterData->corresponds(COMMENTARY_START_CHARACTER)) {
					currentLexem += currentCharacterData->getData();
					lexerState = DELIMETER;
				}

				break;
			case IDENTIFIER:
				if (currentCharacterData->IS(LETTER) || currentCharacterData->IS(DIGIT)) 
					currentLexem += currentCharacterData->getData();
				else if (currentCharacterData->corresponds(SIMPLE_DELIMETER_CHARACTER)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					lexerState = DELIMETER;

					currentLexem.setData(std::string(1, currentCharacterData->getData()));
					currentLexem.setStartingPosition(currentRow, currentColumn);
				}
				else {
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");
				}

				break;
			case CONSTANT:
				if (currentCharacterData->IS(DIGIT))
					currentLexem += currentCharacterData->getData();
				else if (currentCharacterData->corresponds(SIMPLE_DELIMETER_CHARACTER)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					lexerState = DELIMETER;

					currentLexem.setData(std::string(1, currentCharacterData->getData()));
					currentLexem.setStartingPosition(currentRow, currentColumn);
				}
				else if (currentCharacterData->IS(LETTER)) {
					lexerState = ERROR;

					addLog(INVALID_CHARACTER, currentLexem.getStartingPosition());
				}
				else {
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");
				}

				break;
			case DELIMETER:
				if (currentCharacterData->IS(WHITESPACE_CHARACTER, attributesTable)) {
					lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
					currentLexem.setData("");
				}
				else if (currentCharacterData->IS(LETTER)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					currentLexem.setData(std::string(1, currentCharacterData->getData()));
					currentLexem.setStartingPosition(currentRow, currentColumn);

					lexerState = IDENTIFIER;
				}
				else if (currentCharacterData->IS(DIGIT)) {
					encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					currentLexem.setData(std::string(1, currentCharacterData->getData()));
					currentLexem.setStartingPosition(currentRow, currentColumn);

					lexerState = CONSTANT;
				}
				else if (currentCharacterData->corresponds(SIMPLE_DELIMETER_CHARACTER) && !currentCharacterData->corresponds(COMMENTARY_START_CHARACTER)) {
					if (currentCharacterData->IS(SIGN_DELIMETER_CHARACTER) && currentLexem.contains(SIGN_DELIMETER_CHARACTER)) {
							if (IS(currentLexem.getData() + currentCharacterData->getData(), LESS_OR_EQUAL) ||
								IS(currentLexem.getData() + currentCharacterData->getData(), MORE_OR_EQUAL) ||
								IS(currentLexem.getData() + currentCharacterData->getData(), NOT_EQUAL)) {

								currentLexem += currentCharacterData->getData();

								lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState = COMPLEX_DELIMETER);
								currentLexem.setData("");
							}
							else {
								lexerState = ERROR;

								addLog(INVALID_OPERATOR, Position(currentLexem.getStartingPosition()));
							}
					}
					else if (currentLexem.contains(COMMENTARY_BODY_CHARACTER) && IS(currentLexem.getData() + currentCharacterData->getData(), COMMENTARY_END)) {
						lexerState = ERROR;

						addLog(INVALID_OPERATOR, Position(currentLexem.getStartingPosition()));
					}
					else if (!currentLexem.contains(EMPTY_LEXEM) && !currentLexem.contains(COMMENTARY_START_CHARACTER)) {
						lexerState = ERROR;

						addLog(INVALID_OPERATOR, Position(currentLexem.getStartingPosition()));
					}
					else if (currentLexem.contains(COMMENTARY_START_CHARACTER)) {
						if(IS(currentLexem.getData() + currentCharacterData->getData(), COMMENTARY_START)) {
							lexerState = COMMENTARY;

							currentLexem.setData("");
						}
						else {
							lexerState = ERROR;

							addLog(INVALID_OPERATOR, Position(currentLexem.getStartingPosition()));
						}
					}
					else {
						lexerState = encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
						currentLexem.setData("");
					}
				}
				else if (currentCharacterData->corresponds(COMMENTARY_START_CHARACTER)) {
					if (!currentLexem.contains(EMPTY_LEXEM))
						encodeLexem(informationTables, currentLexem, lexemVector, lexerState);

					currentLexem += currentCharacterData->getData();
				}

				break;
			case COMMENTARY:
				if (currentCharacterData->IS(COMMENTARY_BODY_CHARACTER)) {
					if(currentLexem.contains(EMPTY_LEXEM))
						currentLexem += currentCharacterData->getData();
						currentLexem.setData(std::string(1, currentCharacterData->getData()));
				}
				else if (currentCharacterData->IS(COMMENTARY_END_CHARACTER) && !currentLexem.contains(EMPTY_LEXEM)) {
					lexerState = NO_LEXEM_FOUND;
					currentLexem.setData("");
				}
				else if(!currentCharacterData->IS(WHITESPACE_CHARACTER, attributesTable))
					currentLexem.setData("");

				break;
			}
		}

		currentColumn++;
	}

	if (lexerState == COMMENTARY) {
		lexerState = ERROR;

		addLog(UNCLOSED_COMMENTARY, Position(currentLexem.getStartingPosition()));
	}
	else if(lexerState != ERROR)
		encodeLexem(informationTables, currentLexem, lexemVector, lexerState);
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
	case COMMENTARY_START:
		if (lexem == "(*")
			return true;
	case COMMENTARY_END:
		if (lexem == "*)")
			return true;

		return false;
	}
}

LexemString LexicalAnalyzer::getLexemVector() {
	return lexemVector;
}

LexerState LexicalAnalyzer::getState() {
	return lexerState;
}

string LexicalAnalyzer::getLog() {
	return logMessage;
}