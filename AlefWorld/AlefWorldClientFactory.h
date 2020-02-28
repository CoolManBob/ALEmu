#pragma once
#include <iostream>
using std::cout;
using std::endl;

#include "AlefConnectionFactory.h"
#include "AlefWorldPacketHandler.h"
#include "AlefWorldClientConnection.h"
#include "AlefSocket.h"

class AlefWorldClientFactory : public AlefConnectionFactory
{
public:
	AlefWorldClientFactory(AlefWorldPacketHandler* handler) : packetHandler(handler) {};
	virtual ~AlefWorldClientFactory() {};

	virtual AlefWorldClientConnection* createConnection(const StreamSocket& socket)
	{
		cout << "New connection recieved, spawning AlefWorldClientConnection" << endl;
		return new AlefWorldClientConnection(socket, packetHandler);
	}

private:
	AlefWorldPacketHandler * packetHandler;
};