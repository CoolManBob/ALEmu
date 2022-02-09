#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginGlobal.h"
#include "AlefLoginApp.h"
#include "AlefLoginServer.h"

AlefLog* AlefLogger;
AlefLoginConfig* loginConfig;
AlefPacketInterface* pktInterface;
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

	cout << "ALEmu - AlefLogin" << endl;
	
	AlefLoginServer * loginServer = new AlefLoginServer();
	loginServer->runServer();
	
	LOG("HALT!", FATAL);

	loginServer->stopServer();

	waitForTerminationRequest();

	return Application::EXIT_OK;
}