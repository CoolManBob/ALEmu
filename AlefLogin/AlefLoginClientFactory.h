#pragma once

#include "AlefConnectionFactory.h"
#include "AlefLoginPacketHandler.h"
#include "AlefLoginClientConnection.h"

class AlefLoginClientFactory : public AlefConnectionFactory
{
public:
	AlefLoginClientFactory(AlefLoginPacketHandler* handler) : packetHandler(handler) {};
	virtual ~AlefLoginClientFactory() {};

	virtual AlefLoginClientConnection* createConnection(const StreamSocket& socket) { return new AlefLoginClientConnection(socket, packetHandler); };

private:
	AlefLoginPacketHandler * packetHandler;
};