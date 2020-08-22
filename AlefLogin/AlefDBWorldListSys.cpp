#include "AlefDBWorldListSys.h"

AlefDBWorldListSys::AlefDBWorldListSys()
{

}

AlefDBWorldListSys::~AlefDBWorldListSys()
{

}

SharedPtr<RecordSet> AlefDBWorldListSys::dbGetGroupInfo()
{
	try 
	{
		Session dbSession = dbInterface->getLoginDB();

		Statement groupInfo(dbSession);

		groupInfo << "SELECT * FROM worldgroup", now;

		SharedPtr<RecordSet> rs = new RecordSet(groupInfo);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}

SharedPtr<RecordSet> AlefDBWorldListSys::dbGetWorldInfo(int groupID)
{
	try
	{
		Session dbSession = dbInterface->getLoginDB();

		Statement worldInfo(dbSession);

		worldInfo << "SELECT * FROM worldlist WHERE groupID = ?", use(groupID), now;

		SharedPtr<RecordSet> rs = new RecordSet(worldInfo);
		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}