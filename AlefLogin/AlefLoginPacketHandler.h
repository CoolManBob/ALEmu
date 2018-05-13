#pragma once

#include "AlefPacketHandler.h"
#include "AlefLoginStartupEncryption.h"


class AlefLoginPacketHandler : public AlefPacketHandler
{
public:
	AlefLoginPacketHandler() : packetHandler(this, &AlefLoginPacketHandler::packetProcessor) {};
	virtual ~AlefLoginPacketHandler() {};

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

	ActiveMethod<bool, packetInfo, AlefLoginPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;
	HashMap<Int8, AlefPacketProcessor*> processorMap;
};