#pragma once

#include "AlefPacketHandler.h"
#include "AlefTypes.h"
#include "AlefCrypto.h"

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

	ActiveMethod<bool, packetInfo, AlefLoginPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;

private:
	bool packetProcessor(const packetInfo &pktInfo)
	{
		HashMap<Int16, AlefPacketProcessor*>::Iterator Itr = processorMap.find(pktInfo.packet->GetPacketType());
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(pktInfo.sock, pktInfo.packet);
		}
		else
		{
			return false;
		}
	}

	HashMap<Int16, AlefPacketProcessor*> processorMap;
};