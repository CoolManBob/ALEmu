#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldCharacter : public AlefPacketProcessor
{
public:
	AlefWorldCharacter() {};
	virtual ~AlefWorldCharacter() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);

	bool sendCharacterUpdate(AlefSocket& sock, AlefPacket* packet);
	bool sendOptionFlag(AlefSocket& sock, AlefPacket* packet);

};