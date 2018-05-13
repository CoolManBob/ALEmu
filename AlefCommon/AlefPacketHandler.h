#pragma once

#include "Poco/Foundation.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveStarter.h"
#include "Poco/HashMap.h"
#include "Poco/Void.h"

#include "AlefPacketProcessor.h"

using namespace Poco;

struct packetInfo
{
	Int8 PacketType;
	AlefPacket* packet;
};

class AlefPacketHandler : public ActiveDispatcher
{
public:
	AlefPacketHandler() : packetHandler(this, &AlefPacketHandler::packetProcessor) {};
	virtual ~AlefPacketHandler() {};

private:

	bool packetProcessor(const packetInfo &packet)
	{
		HashMap<Int8, AlefPacketProcessor*>::Iterator Itr = processorMap.find(packet.PacketType);
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(packet.packet);
		}
		else
			return false;
	}

	ActiveMethod<bool, packetInfo, AlefPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;
	HashMap<Int8, AlefPacketProcessor*> processorMap;
};