#pragma once
#include <iostream>
using std::cout;
using std::endl;

#include "AlefConnectionFactory.h"
#include "AlefLoginPacketHandler.h"
#include "AlefLoginClientConnection.h"
#include "AlefSocket.h"

class AlefLoginClientFactory : public AlefConnectionFactory
{
public:
	AlefLoginClientFactory(AlefLoginPacketHandler* handler) : packetHandler(handler) {};
	virtual ~AlefLoginClientFactory() {};

	virtual AlefLoginClientConnection* createConnection(const StreamSocket& socket) 
	{ 
		cout << "New connection recieved, spawning AlefLoginClientConnection" << endl;
		return new AlefLoginClientConnection(socket, packetHandler); 
	}

private:
	AlefLoginPacketHandler * packetHandler;
};