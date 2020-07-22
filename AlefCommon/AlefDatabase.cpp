#include "AlefDatabase.h"

AlefDatabase::AlefDatabase()
{
	dbSessionPoolContainer = new SessionPoolContainer();
}

AlefDatabase::~AlefDatabase()
{
	delete dbSessionPoolContainer;
}

void AlefDatabase::createDBSession(std::string connectionString)
{
	//We currently only support MySQL sessions. POCO supports ODBC, and SQLite aswell, but it is unknown at this time if they will eventually be supported.
	dbSessionPool = new SessionPool("MySQL", connectionString);
	dbSessionPoolContainer->add(dbSessionPool);	
}

Session AlefDatabase::getDBSession(std::string sessionAlias)
{
	return dbSessionPoolContainer->get(sessionAlias);
}
