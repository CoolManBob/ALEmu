#include "AlefDBCharSys.h"

AlefDBCharSys::AlefDBCharSys()
{

}

AlefDBCharSys::~AlefDBCharSys()
{

}

SharedPtr<RecordSet> AlefDBCharSys::dbGetAcctID(Int32 authToken)
{
	try
	{
		Session dbSession = dbInterface->getLoginDB();

		Statement acct(dbSession);

		acct << "SELECT acctID FROM account WHERE authToken = ?", use(authToken), now;

		SharedPtr<RecordSet> rs = new RecordSet(acct);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}