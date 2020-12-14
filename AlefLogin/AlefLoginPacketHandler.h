#pragma once

#include "AlefPacketHandler.h"

//Packet Handlers
#include "AlefLoginStartupEncryption.h"
#include "AlefLoginClientLogin.h"
#include "AlefLoginServerList.h"


class AlefLoginPacketHandler : public AlefPacketHandler
{
public:
	AlefLoginPacketHandler() : packetHandler(this, &AlefLoginPacketHandler::packetProcessor) 
	{
		processorMap[Alef::AGPMLOGIN_PACKET_TYPE]				= new AlefLoginClientLogin();
		processorMap[Alef::AGPMWORLD_PACKET_TYPE]				= new AlefLoginServerList();
		processorMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE]	= new AlefLoginStartupEncryption();
	}
	virtual ~AlefLoginPacketHandler() { processorMap.clear(); };

	ActiveMethod<bool, localInfo, AlefLoginPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;

private:
	bool packetProcessor(const localInfo &local)
	{
		HashMap<Int16, AlefPacketProcessor*>::Iterator Itr = processorMap.find(local.packet->GetPacketType());
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(local);
		}
		else
		{
			return false;
		}
	}

	HashMap<Int16, AlefPacketProcessor*> processorMap;
};