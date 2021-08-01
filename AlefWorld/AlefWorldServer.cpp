#include "AlefWorldGlobal.h"
#include "AlefWorldServer.h"
#include "AlefWorldServerSystems.h"

AlefServerEncryptionSys* serverEncryptionSys;
AlefServerCharSys* serverCharSys;
//AlefServerDataSys* serverDataSys;

AlefWorldServer::AlefWorldServer()
{
	handler = new AlefWorldPacketHandler();
	clientFactory = new AlefWorldClientFactory(handler);
	params = new TCPServerParams();
	worldServer = nullptr;
}

AlefWorldServer::~AlefWorldServer()
{
	delete handler;
	delete clientFactory;
	delete worldServer;
}

void AlefWorldServer::runServer()
{
	std::string configLoad = worldConfig->loadConfig();
	LOG(configLoad);

	initDatabase();
	if (!initServerSystems())
	{
		LOG("ERROR: initServerSystems FAIL!", FATAL);
		return;
	}

	timeStamp.update(); //set timeStamp to current time.

	//create worldServer and start it
	worldServer = new TCPServer(clientFactory, worldConfig->getWorldPort());
	worldServer->start();
	LOG("Success: Server Start");

	updateServer();
}

void AlefWorldServer::stopServer()
{
	worldServer->stop();
}

void AlefWorldServer::updateServer()
{
	while (true)
	{
		if (timeStamp.isElapsed(UPDATETIMER)) //Every 60 seconds we will handle any system updates.
		{
			LOG("updateServer", WARNING);

			/*if (!serverListSys->updateWorldList()) //Worldlist refresh error
				break;*/

			timeStamp.update();
		}
	}
}

void AlefWorldServer::initDatabase()
{
	//"host=localhost;port=3306;db=mydb;user=alice;password=s3cr3t;compress=true;auto-reconnect=true"
	std::string connectionStr;
	connectionStr = "host=" + worldConfig->getLoginDBAddress() + ";port=" + worldConfig->getLoginDBPort() + ";db=" + worldConfig->getLoginDB() + ";user=" + worldConfig->getLoginDBUser() + ";password=" + worldConfig->getLoginDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::loginDB, connectionStr);

	connectionStr = "host=" + worldConfig->getWorldDBAddress() + ";port=" + worldConfig->getWorldDBPort() + ";db=" + worldConfig->getWorldDB() + ";user=" + worldConfig->getWorldDBUser() + ";password=" + worldConfig->getWorldDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::worldDB, connectionStr);

	connectionStr = "host=" + worldConfig->getDataDBAddress() + ";port=" + worldConfig->getDataDBPort() + ";db=" + worldConfig->getDataDB() + ";user=" + worldConfig->getDataDBUser() + ";password=" + worldConfig->getDataDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::dataDB, connectionStr);
}

bool AlefWorldServer::initServerSystems()
{
	//serverLoginSys = new AlefServerLoginSys();
	//serverListSys = new AlefServerWorldListSys();
	serverEncryptionSys = new AlefServerEncryptionSys();
	serverCharSys = new AlefServerCharSys();

	/*if (!serverListSys->initWorldList())
	{
		LOG("ERROR: initWorldList FAIL!", FATAL);
		return false;
	}

	serverDataSys = new AlefServerDataSys();

	if (!serverDataSys->initData())
	{
		LOG("ERROR: initData FAIL!", FATAL);
		return false;
	}*/

	LOG("Success: ServerSys Init");
	return true;
}