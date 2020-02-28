#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefLoginGlobal.h"
#include "AlefPacketProcessor.h"
#include "AlefSocket.h"
#include "AlefPacketInterface.h"
#include "AlefCrypto.h"

class AlefLoginStartupEncryption : public AlefPacketProcessor
{
public:
	AlefLoginStartupEncryption() {};
	virtual ~AlefLoginStartupEncryption() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processInitialPacket(AlefSocket& sock, AlefPacket * packet);
	bool processCryptoPacket(AlefSocket& sock, AlefPacket * packet);
};