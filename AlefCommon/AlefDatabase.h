#pragma once

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/SessionPoolContainer.h>

#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>

#include <Poco/AutoPtr.h>
using Poco::AutoPtr;

using Poco::Data::Keywords::use;
using Poco::Data::Keywords::now;

using Poco::Data::Session;
using Poco::Data::SessionPool;
using Poco::Data::SessionPoolContainer;

using Poco::Data::Statement;
using Poco::Data::RecordSet;

class AlefDatabase
{
public:
	AlefDatabase();
	AlefDatabase(std::string connectionString);
	~AlefDatabase();

	void createDBSession(std::string connectionString); //Currently only MySQL is supported.
	Session getDBSession(std::string sessionAlias);

private:
	AutoPtr<SessionPool> dbSessionPool;
	SessionPoolContainer * dbSessionPoolContainer;
};
