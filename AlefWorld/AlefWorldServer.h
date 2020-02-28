#pragma once

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"

using namespace Poco;
using namespace Poco::Net;


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

private:
	AlefWorldClientFactory * clientFactory;
	TCPServer * worldServer;
	AlefWorldPacketHandler * handler;
	TCPServerParams * params;

};