#pragma once

#include "AlefWorldGlobal.h"
#include "AlefWorldTypes.h"

class AlefDBCharSys
{
	friend class AlefServerCharSys;
protected:
	AlefDBCharSys();
	~AlefDBCharSys();

	SharedPtr<RecordSet> dbGetAcctID(Int32 authKey);
	SharedPtr<RecordSet> dbGetChara(UInt32 acctID, string& charName);
};