#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginGlobal.h"
#include "AlefLoginApp.h"
#include "AlefLoginServer.h"

AlefLog* AlefLogger;
AlefPacketInterface* pktInterface;

int AlefLoginApp::main(const vector<string>& args)
{
	/*cout << "|----------------------------------------------------------------|" << endl
		 <<  "|							ALEmu                                 |" << endl
		 <<  "|						  AlefLogin							      |" << endl
		 <<  "|                          v0.1								  |" << endl
		 <<  "|----------------------------------------------------------------|" << endl;*/

	AlefLogger = new AlefLog("AlefLogin.log", "AlefLogin");
	pktInterface = new AlefPacketInterface();

	cout << "ALEmu - AlefLogin v0.1" << endl;
	AlefLoginServer * loginServer = new AlefLoginServer();
	loginServer->runServer();
	
	LOG("Server Start");

	waitForTerminationRequest();

	LOG("Server Stopped");
	
	loginServer->stopServer();

	return Application::EXIT_OK;
}