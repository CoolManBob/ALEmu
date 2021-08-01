#include "AlefWorldServerSystems.h"

#define SINGLEROW 0 //For single 0th row values

AlefServerCharSys::AlefServerCharSys()
{
	dbCharSys = new AlefDBCharSys();
}

AlefServerCharSys::~AlefServerCharSys()
{
	delete dbCharSys;
}

UInt32 AlefServerCharSys::getAcctID(Int32 authKey)
{
	UInt32 acctID = -1;
	SharedPtr<RecordSet> rs = dbCharSys->dbGetAcctID(authKey);

	if (!rs)
		return 0;
	if (rs->getTotalRowCount() != 1) //We should only get one row returned
		return 0;

	acctID = rs->row(SINGLEROW).get(COLIDX(ACCOUNTCOL::ACCTID));

	return acctID;
}
