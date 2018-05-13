#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginApp.h"
#include "AlefLoginServer.h"

int AlefLoginApp::main(const vector<string>& args)
{
	/*cout << "|----------------------------------------------------------------|" << endl
		 <<  "|							ALEmu                                 |" << endl
		 <<  "|						  AlefLogin							      |" << endl
		 <<  "|                          v0.1								  |" << endl
		 <<  "|----------------------------------------------------------------|" << endl;*/

	cout << "ALEmu - AlefLogin v0.1" << endl;
	AlefLoginServer * loginServer = new AlefLoginServer();
	loginServer->runServer();

	cout << "Server started..." << endl;

	waitForTerminationRequest();

	cout << "Stopping server..." << endl;

	loginServer->stopServer();

	return Application::EXIT_OK;
}