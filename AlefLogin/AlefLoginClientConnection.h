#pragma once

#include "AlefServerConnection.h"
#include "AlefLoginPacketHandler.h"

class AlefLoginClientConnection : public AlefServerConnection
{
public:
	//AlefLoginClientConnection() {};
	AlefLoginClientConnection(const StreamSocket& socket, AlefLoginPacketHandler* packetHandler) : AlefServerConnection(socket, handler), handler(packetHandler) {};
	virtual ~AlefLoginClientConnection() {};

	virtual void run() {};
private:
	AlefLoginPacketHandler * handler;
};