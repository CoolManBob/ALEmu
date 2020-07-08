#include "AlefWorldServer.h"

AlefWorldServer::AlefWorldServer()
{
	handler = new AlefWorldPacketHandler();
	clientFactory = new AlefWorldClientFactory(handler);
	params = new TCPServerParams();
	worldServer = new TCPServer(clientFactory, 11008);
}

AlefWorldServer::~AlefWorldServer()
{

}

void AlefWorldServer::runServer()
{
	worldServer->start();

	int tickCount = 0;
	while (true)
	{
		if (tickCount == 500000000)
		{
			LOG("TICK COUNT 500000000");
			tickCount = 0;
		}

		tickCount++;
	}
}

void AlefWorldServer::stopServer()
{
	worldServer->stop();
}