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
};