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

	enum ACCOUNTCOL
	{
		ACCTID,
		ACCTRNAME,
		ACCTPW
	};

};