#pragma once

#include "AlefPacketHandler.h"
#include "AlefLoginStartupEncryption.h"
#include "AlefPacketTypes.h"
#include "AlefCrypto.h"


class AlefLoginPacketHandler : public AlefPacketHandler
{
public:
	AlefLoginPacketHandler() : packetHandler(this, &AlefLoginPacketHandler::packetProcessor) 
	{
		processorMap[AGPMSTARTUPENCRYPTION_PACKET_TYPE] = new AlefLoginStartupEncryption();
	}
	virtual ~AlefLoginPacketHandler() {};

	ActiveMethod<bool, packetInfo, AlefLoginPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;

private:
	bool packetProcessor(const packetInfo &packet)
	{
		HashMap<Int8, AlefPacketProcessor*>::Iterator Itr = processorMap.find(packet.PacketType);
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(packet.sock, packet.packet);
		}
		else
		{
			return false;
		}
	}

	HashMap<Int8, AlefPacketProcessor*> processorMap;
};