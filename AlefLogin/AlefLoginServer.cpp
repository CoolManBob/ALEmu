#include "AlefLoginGlobal.h"
#include "AlefLoginServer.h"

AlefServerLoginSys* serverLoginSys;
AlefServerWorldListSys* serverListSys;
AlefServerEncryptionSys* serverEncryptionSys;

AlefLoginServer::AlefLoginServer()
{
	handler = new AlefLoginPacketHandler();
	clientFactory = new AlefLoginClientFactory(handler);
	params = new TCPServerParams();
	loginServer = new TCPServer(clientFactory, 11002);
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
	initServerSystems();

	loginServer->start();
}

void AlefLoginServer::stopServer()
{
	loginServer->stop();
}

void AlefLoginServer::initDatabase()
{
	//"host=localhost;port=3306;db=mydb;user=alice;password=s3cr3t;compress=true;auto-reconnect=true"
	std::string connectionStr;
	connectionStr = "host=" + loginConfig->getLoginDBAddress() + ";port=3306;db=" + loginConfig->getLoginDB() + ";user=" + loginConfig->getLoginDBUser() + ";password=" + loginConfig->getLoginDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::loginDB, connectionStr);

	connectionStr = "host=" + loginConfig->getWorldDBAddress() + ";port=3306;db=" + loginConfig->getWorldDB() + ";user=" + loginConfig->getWorldDBUser() + ";password=" + loginConfig->getWorldDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::worldDB, connectionStr);

	connectionStr = "host=" + loginConfig->getDataDBAddress() + ";port=3306;db=" + loginConfig->getDataDB() + ";user=" + loginConfig->getDataDBUser() + ";password=" + loginConfig->getDataDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::dataDB, connectionStr);

	LOG("Database Init Finished");
}

void AlefLoginServer::initServerSystems()
{
	serverLoginSys = new AlefServerLoginSys();
	serverListSys = new AlefServerWorldListSys();
	serverEncryptionSys = new AlefServerEncryptionSys();

	LOG("Server System Init Finished");
}

