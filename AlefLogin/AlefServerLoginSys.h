#pragma once

#include "AlefDBLoginSys.h"

class AlefServerLoginSys
{
public:
	AlefServerLoginSys();
	~AlefServerLoginSys();

	bool checkLogin(string username, string password, UInt32& acctID);
	
	bool getCharData(UInt32 acctID, clientCharDataVec & charData);
	bool checkCharName(string charName);
	bool createChar(UInt32 acctID, UInt32 tID, UInt32 face, UInt32 hair, string charName);
	bool deleteChar(UInt32 acctID, string charName);

private:
	AlefDBLoginSys* dbLoginSys;

	bool initStatusFactor(SharedPtr<CharacterData>& charData);
	bool initTypeFactor(SharedPtr<CharacterData>& charData);
	bool initPointMaxFactor(SharedPtr<CharacterData>& charData);
	bool initAttackFactor(SharedPtr<CharacterData>& charData);

	enum class ACCOUNTCOL
	{
		ACCTID,
		ACCTRNAME,
		ACCTPW
	};

	enum class CHARCOL
	{
		CHARID,
		ACCTID,
		CHARNAME,
		CHARTID,
		SLOT,
		POSX,
		POSY,
		POSZ,
		HP,
		MP,
		SP,
		EXP,
		LEVEL,
		SKILLPOINT,
		INVENMONEY,
		HAIR,
		FACE,
	};
};