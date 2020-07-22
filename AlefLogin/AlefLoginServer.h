#pragma once

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"

using namespace Poco;
using namespace Poco::Net;


#include "AlefServer.h"
#include "AlefLoginClientFactory.h"
#include "AlefLoginPacketHandler.h"

class AlefLoginServer : public AlefServer
{
public:
	AlefLoginServer();
	virtual ~AlefLoginServer();

	virtual void runServer();
	void stopServer();

private:
	void initDatabase();
	void initServerSystems();

	AlefLoginClientFactory * clientFactory;
	TCPServer * loginServer;
	AlefLoginPacketHandler * handler;
	TCPServerParams * params;

};