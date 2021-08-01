#include "AlefDBLoginSys.h"

AlefDBLoginSys::AlefDBLoginSys()
{

}

AlefDBLoginSys::~AlefDBLoginSys()
{

}

SharedPtr<RecordSet> AlefDBLoginSys::dbGetAccount(string username, string password)
{
	try
	{
		Session dbSession = dbInterface->getLoginDB();

		Statement acctInfo(dbSession);

		acctInfo << "SELECT * FROM account WHERE acctName = ? AND acctPW = ?", use(username), use(password), now;

		SharedPtr<RecordSet> rs = new RecordSet(acctInfo);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}

SharedPtr<RecordSet> AlefDBLoginSys::dbGetCharacters(UInt32 acctID)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charData(dbSession);

		charData << "SELECT * FROM character_data WHERE acctID = ?", use(acctID), now;

		SharedPtr<RecordSet> rs = new RecordSet(charData);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}

SharedPtr<RecordSet> AlefDBLoginSys::dbGetCharacterFromSlot(UInt32 acctID, UInt32 slot)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charData(dbSession);

		charData << "SELECT * FROM character_data WHERE acctID = ? AND slot = ?", use(acctID), use(slot), now;

		SharedPtr<RecordSet> rs = new RecordSet(charData);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}

bool AlefDBLoginSys::dbCreateCharacter(UInt32 acctID, UInt32 tID, UInt32 face, UInt32 hair, string charName)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charCreate(dbSession);

		charCreate << "call createCharacter(?,?,?,?,?);" , in(acctID), in(tID), in(face), in(hair), in(charName), now;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return false;
	}

	return true;
}

bool AlefDBLoginSys::dbDeleteCharacter(UInt32 acctID, string charName)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charDelete(dbSession);

		charDelete << "DELETE FROM character_data WHERE acctID = ? AND charName = ?", in(acctID), in(charName), now;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return false;
	}

	return true;
}

SharedPtr<RecordSet> AlefDBLoginSys::dbCheckCharName(string charName)
{
	try
	{
		Session dbSession = dbInterface->getWorldDB();

		Statement charData(dbSession);

		charData << "SELECT * FROM character_data WHERE charName = ?", bind(charName), now;

		SharedPtr<RecordSet> rs = new RecordSet(charData);

		return rs;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return nullptr;
	}
}

bool AlefDBLoginSys::dbSetAuthKey(UInt32 acctID, Int32 authKey)
{
	try
	{
		Session dbSession = dbInterface->getLoginDB();

		Statement authSetup(dbSession);

		authSetup << "UPDATE account SET authToken = ? WHERE acctID = ?", use(authKey), use(acctID), now;
	}
	catch (const Poco::Exception& ex)
	{
		LOG(ex.displayText(), FATAL);
		return false;
	}

	return true;
}
