#include "AlefDBInterface.h"

#include <iostream>

AlefDBInterface::AlefDBInterface()
{
	Poco::Data::MySQL::Connector::registerConnector();

	database[	dbType::loginDB	] = new AlefDatabase();
	database[	dbType::worldDB	] = new AlefDatabase();
	database[	dbType::dataDB	] = new AlefDatabase();
}

AlefDBInterface::~AlefDBInterface()
{
	delete[] &database;
}

bool AlefDBInterface::createDatabaseConnection(dbType type, std::string connectionString)
{
	if (type > 2 || type < 0)
		return false;

	sessionNames[type] = "MySQL:///" + connectionString;

	database[type]->createDBSession(connectionString);

	return true;
}

Session AlefDBInterface::acquireDatabaseSession(dbType type)
{
	return database[type]->getDBSession(sessionNames[type]);
}

void AlefDBInterface::test()
{
	try {
		Session sess = database[0]->getDBSession(sessionNames[0]);

		Statement insert(sess);

		struct acct
		{
			int id;
			std::string name;
			std::string pw;
		};

		// insert some rows
		acct testacct =
		{
			2,
			"testinsert",
			"testinsert"
		};

		insert << "INSERT INTO account VALUES(?, ?, ?)",
			use(testacct.id),
			use(testacct.name),
			use(testacct.pw);

		insert.execute();
	}
	catch (const Poco::Exception& ex)
	{
		std::cout << ex.displayText() << std::endl;
	}
}