#pragma once

#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"

#include "AlefPacketHandler.h"

using namespace Poco::Net;

class AlefServerConnection : public TCPServerConnection
{
public:
	//AlefServerConnection() {};
	AlefServerConnection(const StreamSocket& socket, AlefPacketHandler* packetHandler) : TCPServerConnection(socket), handler(packetHandler) {};
	virtual ~AlefServerConnection() {};

	virtual void start() {};
	virtual void run() {};
private:
	AlefPacketHandler* handler;
};