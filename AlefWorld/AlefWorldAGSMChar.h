#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldAGSMChar : public AlefPacketProcessor
{
public:
	AlefWorldAGSMChar() {};
	virtual ~AlefWorldAGSMChar() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processGameEnterCharacterName(AlefSocket& sock, AlefPacket* packet);
	bool processEnterWorld(AlefSocket& sock, AlefPacket* packet);

	void sendDummyCharacter(AlefSocket& sock);
	
};