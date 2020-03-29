#pragma once

#include "AlefPacketHandler.h"
#include "AlefTypes.h"
#include "AlefCrypto.h"

//Packet Handlers
#include "AlefWorldCharacter.h"
#include "AlefWorldAGSMCharMgr.h"
#include "AlefWorldUIStatus.h"
#include "AlefWorldOptimizedCharMove.h"
#include "AlefWorldStartupEncryption.h"


class AlefWorldPacketHandler : public AlefPacketHandler
{
public:
	AlefWorldPacketHandler() : packetHandler(this, &AlefWorldPacketHandler::packetProcessor)
	{
		processorMap[Alef::AGPMCHARACTER_PACKET_TYPE]			= new AlefWorldCharacter();
		processorMap[Alef::AGSMCHARMANAGER_PACKET_TYPE]			= new AlefWorldAGSMCharMgr();
		processorMap[Alef::AGPMUISTATUS_PACKET_TYPE]			= new AlefWorldUIStatus();
		processorMap[Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE]	= new AlefWorldOptimizedCharMove();
		processorMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE]	= new AlefWorldStartupEncryption();
	}
	virtual ~AlefWorldPacketHandler() {};

	ActiveMethod<bool, packetInfo, AlefWorldPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;

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