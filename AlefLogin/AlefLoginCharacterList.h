#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefLoginCharacterList : public AlefPacketProcessor
{
public:
	AlefLoginCharacterList() {};
	virtual ~AlefLoginCharacterList() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
};