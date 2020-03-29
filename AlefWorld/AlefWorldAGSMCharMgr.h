#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldAGSMCharMgr : public AlefPacketProcessor
{
public:
	AlefWorldAGSMCharMgr() {};
	virtual ~AlefWorldAGSMCharMgr() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processGameEnterCharacterName(AlefSocket& sock, AlefPacket* packet);
	bool processEnterWorld(AlefSocket& sock, AlefPacket* packet);

	void sendDummyCharacter(AlefSocket& sock);
	
};