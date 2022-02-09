#pragma once

#include "AlefDBCharSys.h"

class AlefServerCharSys
{
public:
	AlefServerCharSys();
	~AlefServerCharSys();

	UInt32 getAcctID(Int32 authKey);
	CharacterData getCharacter(UInt32 acctID, string& charName);

private:

	void initCharFactors(CharacterData& charData);

	void initStatusFactor(CharacterData& charData);
	void initTypeFactor(CharacterData& charData);
	void initPointMaxFactor(CharacterData& charData);
	void initAttackFactor(CharacterData& charData);

	AlefDBCharSys* dbCharSys;

	enum class ACCOUNTCOL
	{
		ACCTID,
		ACCTRNAME,
		ACCTPW,
		AUTHKEY
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