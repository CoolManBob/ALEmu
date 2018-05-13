#pragma once

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/ServerSocket.h"

#include "AlefConnectionFactory.h"

using namespace Poco::Net;

class AlefServer
{
public:
	AlefServer() {};
	virtual ~AlefServer() {};

	virtual void runServer() {};

private:
	AlefConnectionFactory * connectFactory;
};