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

	//DB
	enum GROUPCOL : Int32
	{
		GROUPIDCOL = 0,
		GROUPNAMECOL
	};

	enum WORLDCOL : Int32
	{
		WORLDIDCOL = 0,
		WORLDGRPIDCOL,
		WORLDNAMECOL,
		WORLDADDRCOL,
		WORLDSTATUSCOL,
		WORLDFLAGCOL
	};	
};