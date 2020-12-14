#pragma once

#include "Poco/Foundation.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveStarter.h"
#include "Poco/HashMap.h"
#include "Poco/Void.h"

#include "AlefLocalInfo.h"

#include "AlefPacketProcessor.h"
#include "AlefCrypto.h"

using namespace Poco;

class AlefPacketHandler : public ActiveDispatcher
{
public:
	AlefPacketHandler() : packetHandler(this, &AlefPacketHandler::packetProcessor) {};
	virtual ~AlefPacketHandler() {};

private:

	bool packetProcessor(const localInfo &local)
	{
		HashMap<Int16, AlefPacketProcessor*>::Iterator Itr = processorMap.find(local.packet->GetPacketType());
		if (Itr != processorMap.end())
		{
			return Itr->second->processPacket(local);
			//return Itr->second->processPacket(pktInfo.sock, pktInfo.packet);
		}
		else
			return false;
	}

	ActiveMethod<bool, localInfo, AlefPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;
	HashMap<Int16, AlefPacketProcessor*> processorMap;
};