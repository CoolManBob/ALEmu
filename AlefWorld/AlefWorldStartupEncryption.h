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

	virtual bool processPacket(const localInfo& local);
	bool processInitialPacket(localInfo& local);
	bool processCryptoPacket(localInfo& local);
};