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

	virtual bool processPacket(const localInfo& local);

	bool sendCharacterUpdate(localInfo& local);
	bool sendOptionFlag(localInfo& local);

};