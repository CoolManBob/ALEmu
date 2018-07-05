#pragma once

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"

#include "AlefPacketHandler.h"
#include "AlefServerConnection.h"
#include "AlefSocket.h"

using namespace Poco::Net;

class AlefConnectionFactory : public TCPServerConnectionFactory
{
public:
	AlefConnectionFactory() {};
	AlefConnectionFactory(AlefPacketHandler* packetHandler) : handler(packetHandler) {};
	virtual ~AlefConnectionFactory() {};

	virtual AlefServerConnection* createConnection(const StreamSocket& socket) { return new AlefServerConnection(socket, handler); };

private:
	AlefPacketHandler* handler;
};