#pragma once

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"

using namespace Poco;
using namespace Poco::Net;

#define UPDATETIMEFACTOR 1000000 //1 second
#define UPDATETIMESECONDS 60 //60 seconds
#define UPDATETIMER UPDATETIMESECONDS*UPDATETIMEFACTOR

#include "AlefServer.h"
#include "AlefWorldClientFactory.h"
#include "AlefWorldPacketHandler.h"

class AlefWorldServer : public AlefServer
{
public:
	AlefWorldServer();
	virtual ~AlefWorldServer();

	virtual void runServer();
	void stopServer();
	void updateServer();

private:
	void initDatabase();
	bool initServerSystems();

	Timestamp timeStamp;
	AlefWorldClientFactory * clientFactory;
	TCPServer * worldServer;
	AlefWorldPacketHandler * handler;
	TCPServerParams * params;

};