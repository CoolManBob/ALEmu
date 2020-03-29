#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldUIStatus : public AlefPacketProcessor
{
public:
	AlefWorldUIStatus() {};
	virtual ~AlefWorldUIStatus() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);

};