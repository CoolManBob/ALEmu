#include "AlefLoginServer.h"

AlefLoginServer::AlefLoginServer()
{
	handler = new AlefLoginPacketHandler();
	clientFactory = new AlefLoginClientFactory(handler);
	params = new TCPServerParams();
	loginServer = new TCPServer(clientFactory, 11002);
}

AlefLoginServer::~AlefLoginServer()
{

}

void AlefLoginServer::runServer()
{
	loginServer->start();
}

void AlefLoginServer::stopServer()
{
	loginServer->stop();
}