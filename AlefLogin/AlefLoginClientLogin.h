#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefLoginClientLogin : public AlefPacketProcessor
{
public:
	AlefLoginClientLogin() {};
	virtual ~AlefLoginClientLogin() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processInitialLoginPacket(AlefSocket& sock, AlefPacket* packet);
	bool processUserLoginPacket(AlefSocket& sock, AlefPacket* packet);
};