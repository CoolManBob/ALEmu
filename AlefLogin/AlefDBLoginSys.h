#pragma once

#include "AlefLoginGlobal.h"
#include "AlefLoginTypes.h"

class AlefDBLoginSys
{
	friend class AlefServerLoginSys;
protected:
	AlefDBLoginSys();
	~AlefDBLoginSys();

	SharedPtr<RecordSet> dbGetAccount(std::string username, std::string password);
	SharedPtr<RecordSet> dbGetCharacters(UInt32 acctID);
	SharedPtr<RecordSet> dbGetCharacterFromSlot(UInt32 acctID, UInt32 slot);
	bool dbCreateCharacter(UInt32 acctID, UInt32 tID, UInt32 face, UInt32 hair, string charName);
	bool dbDeleteCharacter(UInt32 acctID, string charName);
	SharedPtr<RecordSet> dbCheckCharName(string charName);
};