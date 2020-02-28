#pragma once

#include "AlefPacketHandler.h"
#include "AlefTypes.h"
#include "AlefCrypto.h"

//Packet Handlers
#include "AlefWorldStartupEncryption.h"
#include "AlefWorldAGSMChar.h"
//#include "AlefWorldClientLogin.h"
//#include "AlefWorldServerList.h"


class AlefWorldPacketHandler : public AlefPacketHandler
{
public:
	AlefWorldPacketHandler() : packetHandler(this, &AlefWorldPacketHandler::packetProcessor)
	{
		processorMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE] = new AlefWorldStartupEncryption();
		processorMap[Alef::AGSMCHARMANAGER_PACKET_TYPE] = new AlefWorldAGSMChar();
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