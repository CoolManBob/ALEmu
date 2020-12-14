#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefWorldOptimizedCharMove : public AlefPacketProcessor
{
public:
	AlefWorldOptimizedCharMove() {};
	virtual ~AlefWorldOptimizedCharMove() {};

	virtual bool processPacket(const localInfo& local);

};