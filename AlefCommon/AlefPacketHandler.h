#pragma once

#include "Poco/Foundation.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveStarter.h"
#include "Poco/HashMap.h"
#include "Poco/Void.h"

#include "AlefPacketProcessor.h"
#include "AlefCrypto.h"
#include "AlefSocket.h"

using namespace Poco;

struct packetInfo //TODO: This needs to be moved
{
	packetInfo(AlefSocket& socket) : sock(socket), packet(nullptr) {};
	AlefPacket* packet;
	AlefSocket & sock;
};

class AlefPacketHandler : public ActiveDispatcher
{
public:
	AlefPacketHandler() : packetHandler(this, &AlefPacketHandler::packetProcessor) {};
	virtual ~AlefPacketHandler() {};

private:

	bool packetProcessor(const packetInfo &pktInfo)
	{
		HashMap<Int16, AlefPacketProcessor*>::Iterator Itr = processorMap.find(pktInfo.packet->GetPacketType());
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(pktInfo.sock, pktInfo.packet);
		}
		else
			return false;
	}

	ActiveMethod<bool, packetInfo, AlefPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;
	HashMap<Int16, AlefPacketProcessor*> processorMap;
};