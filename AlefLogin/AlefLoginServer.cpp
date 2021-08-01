#include "AlefLoginGlobal.h"
#include "AlefLoginServer.h"
#include "AlefLoginServerSystems.h"

AlefServerLoginSys* serverLoginSys;
AlefServerWorldListSys* serverListSys;
AlefServerEncryptionSys* serverEncryptionSys;
AlefServerDataSys* serverDataSys;

AlefLoginServer::AlefLoginServer()
{
	handler = new AlefLoginPacketHandler();
	clientFactory = new AlefLoginClientFactory(handler);
	params = new TCPServerParams();
	loginServer = nullptr;
}

AlefLoginServer::~AlefLoginServer()
{
	delete handler;
	delete clientFactory;
	delete loginServer;
}

void AlefLoginServer::runServer()
{
	std::string configLoad = loginConfig->loadConfig();
	LOG(configLoad);

	initDatabase();
	if (!initServerSystems())
	{
		LOG("ERROR: initServerSystems FAIL!", FATAL);
		return;
	}
	
	timeStamp.update(); //set timeStamp to current time.

	//create loginServer and start it
	loginServer = new TCPServer(clientFactory, loginConfig->getLoginPort());
	loginServer->start();
	LOG("Success: Server Start");

	updateServer();
}

void AlefLoginServer::stopServer()
{
	loginServer->stop();
	LOG("Success: Server Stop");
}

void AlefLoginServer::updateServer()
{
	while (true)
	{
		if (timeStamp.isElapsed(UPDATETIMER)) //Every 60 seconds we will handle any system updates.
		{
			LOG("updateServer", WARNING);
			
			if (!serverListSys->updateWorldList()) //Worldlist refresh error
				break;

			timeStamp.update();
		}
	}
}

void AlefLoginServer::initDatabase()
{
	//"host=localhost;port=3306;db=mydb;user=alice;password=s3cr3t;compress=true;auto-reconnect=true"
	std::string connectionStr;
	connectionStr = "host=" + loginConfig->getLoginDBAddress() + ";port=" + loginConfig->getLoginDBPort() + ";db=" + loginConfig->getLoginDB() + ";user=" + loginConfig->getLoginDBUser() + ";password=" + loginConfig->getLoginDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::loginDB, connectionStr);

	connectionStr = "host=" + loginConfig->getWorldDBAddress() + ";port=" + loginConfig->getWorldDBPort() + ";db=" + loginConfig->getWorldDB() + ";user=" + loginConfig->getWorldDBUser() + ";password=" + loginConfig->getWorldDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::worldDB, connectionStr);

	connectionStr = "host=" + loginConfig->getDataDBAddress() + ";port=" + loginConfig->getDataDBPort() + ";db=" + loginConfig->getDataDB() + ";user=" + loginConfig->getDataDBUser() + ";password=" + loginConfig->getDataDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::dataDB, connectionStr);
}

bool AlefLoginServer::initServerSystems()
{
	serverLoginSys = new AlefServerLoginSys();
	serverListSys = new AlefServerWorldListSys();
	serverEncryptionSys = new AlefServerEncryptionSys();

	if (!serverListSys->initWorldList())
	{
		LOG("ERROR: initWorldList FAIL!", FATAL);
		return false;
	}

	serverDataSys = new AlefServerDataSys();

	if (!serverDataSys->initData())
	{
		LOG("ERROR: initData FAIL!", FATAL);
		return false;
	}

	LOG("Success: ServerSys Init");
	return true;
}

