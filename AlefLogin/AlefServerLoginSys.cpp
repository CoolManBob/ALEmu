#include "AlefServerLoginSys.h"

AlefServerLoginSys::AlefServerLoginSys()
{
	dbLoginSys = new AlefDBLoginSys();
}

AlefServerLoginSys::~AlefServerLoginSys()
{
	delete dbLoginSys;
}

bool AlefServerLoginSys::checkLogin(string username, string password)
{
	SharedPtr<RecordSet> rs = dbLoginSys->dbGetAccount(username, password);

	if (!rs)
		return false;

	if (rs->getTotalRowCount() == 0)
	{
		LOG("getGroupInfo() row count : 0", FATAL);
		return false;
	}
}