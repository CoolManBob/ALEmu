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
	packetInfo(AlefSocket& socket/*, blowfish_session& session*/) : sock(socket)/*, cryptoSession(session)*/ {};
	Int8 PacketType;
	Int8 PacketFlag;
	Int8 PacketOperation;
	AlefPacket* packet;
	//blowfish_session* cryptoSession;
	AlefSocket & sock;
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
			return Itr->second->processPacket(packet.sock, packet.packet);
		}
		else
			return false;
	}

	ActiveMethod<bool, packetInfo, AlefPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;
	HashMap<Int8, AlefPacketProcessor*> processorMap;
};