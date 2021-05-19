#pragma once

#include "AlefLocalTypes.h"

class AlefLocalSys
{
public:
	AlefLocalSys();
	~AlefLocalSys();

	AlefClientAccount* _localAcct() { return localAcct; }
	AlefClientCharacter* _localChar() { return localChar; }

	void setAcctID(UInt32 acctID)
	{
		localAcct->setUserID(acctID);
		localChar->setAcctID(acctID);
	}

private:
	AlefClientAccount* localAcct;
	AlefClientCharacter* localChar;
};