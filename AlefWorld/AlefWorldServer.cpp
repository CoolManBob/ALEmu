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
}

void AlefWorldServer::stopServer()
{
	worldServer->stop();
}