#pragma once

#include "AlefClientCharacterTypes.h"

class AlefClientCharacter
{
public:
	AlefClientCharacter();
	~AlefClientCharacter();

	void setCharData(clientCharDataVec charData) { characters = charData; }
	clientCharDataVec getCharacters() { return characters; }

	void setCurChar(CharacterData charData) { curChar = charData; }
	CharacterData getCurrentChar() { return curChar; }
	CharacterData getCharAtSlot(Int32 slotIdx);
	UInt32 getCharCount() { return (UInt32)characters.size(); }
	UInt32 getAvailableSlotNum();

	void setAcctID(UInt32 ID) { acctID = ID; }


private:
	clientCharDataVec characters;
	CharacterData curChar;
	UInt32 acctID;
};