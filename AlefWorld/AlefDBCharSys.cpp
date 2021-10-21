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

SharedPtr<RecordSet> AlefDBCharSys::dbGetChara(UInt32 acctID, string& charName)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charData(dbSession);

		charData << "SELECT * FROM character_data WHERE acctID = ? AND charName = ?", use(acctID), bind(charName), now;

		SharedPtr<RecordSet> rs = new RecordSet(charData);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}