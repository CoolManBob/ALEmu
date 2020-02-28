#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldStartupEncryption : public AlefPacketProcessor
{
public:
	AlefWorldStartupEncryption() {};
	virtual ~AlefWorldStartupEncryption() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processInitialPacket(AlefSocket& sock, AlefPacket* packet);
	bool processCryptoPacket(AlefSocket& sock, AlefPacket* packet);
};