#pragma once

#include "AlefPacket.h"
#include "AlefPacketInterface.h"
#include "AlefClientCharacterTypes.h"

extern AlefPacketInterface* pktInterface;

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

	UInt32 getCharID() { return curChar.charID; }
	void setCharID(UInt32 ID) { curChar.charID = ID; }

	Alef::AlefVec3F getCurPos() { return curChar.charMove.curPos; }

	SharedPtr<AlefPacket> getCharPkt() { return buildCharPkt(curChar); }
	SharedPtr<AlefPacket> getCharMovePkt() { return buildCharMovePacket(curChar.charMove); }
	SharedPtr<AlefPacket> getCharFactorPkt() { return buildFactorPacket(curChar.charFactors); }

private:
	SharedPtr<AlefPacket> buildCharPkt(CharacterData& data);
	SharedPtr<AlefPacket> buildCharMovePacket(CharacterMove& move);
	SharedPtr<AlefPacket> buildFactorPacket(CharacterFactors& factors);
	SharedPtr<AlefPacket> buildFactorResultPacket(CharacterFactorResult& result);

	clientCharDataVec characters;
	CharacterData curChar;
	UInt32 acctID;
};