#pragma once

#include "AlefLoginGlobal.h"
#include "AlefServerWorldListTypes.h"

class AlefDBWorldListSys
{
	friend class AlefServerWorldListSys;
protected:
	AlefDBWorldListSys();
	~AlefDBWorldListSys();

	SharedPtr<RecordSet> dbGetGroupInfo();
	SharedPtr<RecordSet> dbGetWorldInfo(int groupID);
};