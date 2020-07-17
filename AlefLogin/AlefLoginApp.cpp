#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginGlobal.h"
#include "AlefLoginApp.h"
#include "AlefLoginServer.h"

AlefLog* AlefLogger;
AlefPacketInterface* pktInterface;
AlefLoginConfig* loginConfig;
AlefDBInterface* dbInterface;

int AlefLoginApp::main(const vector<string>& args)
{
	/*cout << "|----------------------------------------------------------------|" << endl
		 <<  "|							ALEmu                                 |" << endl
		 <<  "|						  AlefLogin							      |" << endl
		 <<  "|                          v0.1								  |" << endl
		 <<  "|----------------------------------------------------------------|" << endl;*/

	AlefLogger = new AlefLog("AlefLogin.log", "AlefLogin");
	loginConfig = new AlefLoginConfig("AlefLogin.ini");
	pktInterface = new AlefPacketInterface();
	dbInterface = new AlefDBInterface();

	initDatabase();

	cout << "ALEmu - AlefLogin v0.1" << endl;
	AlefLoginServer * loginServer = new AlefLoginServer();
	loginServer->runServer();
	
	LOG("Server Start");

	waitForTerminationRequest();

	LOG("Server Stopped");
	
	loginServer->stopServer();

	return Application::EXIT_OK;
}

void AlefLoginApp::initDatabase()
{
	//"host=localhost;port=3306;db=mydb;user=alice;password=s3cr3t;compress=true;auto-reconnect=true"
	std::string connectionStr;
	connectionStr = "host=" + loginConfig->getLoginDBAddress() + ";port=3306;db=" + loginConfig->getLoginDB() + ";user=" + loginConfig->getLoginDBUser() + ";password=" + loginConfig->getLoginDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::loginDB,connectionStr);

	connectionStr = "host=" + loginConfig->getWorldDBAddress() + ";port=3306;db=" + loginConfig->getWorldDB() + ";user=" + loginConfig->getWorldDBUser() + ";password=" + loginConfig->getWorldDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::worldDB, connectionStr);

	connectionStr = "host=" + loginConfig->getDataDBAddress() + ";port=3306;db=" + loginConfig->getDataDB() + ";user=" + loginConfig->getDataDBUser() + ";password=" + loginConfig->getDataDBPass();

	dbInterface->createDatabaseConnection(AlefDBInterface::dbType::dataDB, connectionStr);
}