#include <iostream>
using std::cout;
using std::endl;

#include "AlefWorldGlobal.h"
#include "AlefWorldApp.h"
#include "AlefWorldServer.h"

AlefLog* AlefLogger;

int AlefWorldApp::main(const vector<string>& args)
{
	/*cout << "|----------------------------------------------------------------|" << endl
	<<  "|							ALEmu                                 |" << endl
	<<  "|						  AlefWorld							      |" << endl
	<<  "|                          v0.1								  |" << endl
	<<  "|----------------------------------------------------------------|" << endl;*/

	AlefLogger = new AlefLog("AlefWorld.log", "AlefWorld");

	cout << "ALEmu - AlefWorld v0.1" << endl;
	AlefWorldServer * worldServer = new AlefWorldServer();
	worldServer->runServer();

	LOG("Server Start");

	waitForTerminationRequest();

	LOG("Server Stopped");

	worldServer->stopServer();

	return Application::EXIT_OK;
}