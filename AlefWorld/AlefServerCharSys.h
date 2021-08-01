#pragma once

#include "AlefDBCharSys.h"

class AlefServerCharSys
{
public:
	AlefServerCharSys();
	~AlefServerCharSys();

	UInt32 getAcctID(Int32 authKey);

private:
	AlefDBCharSys* dbCharSys;

	enum class ACCOUNTCOL
	{
		ACCTID,
		ACCTRNAME,
		ACCTPW,
		AUTHKEY
	};
};