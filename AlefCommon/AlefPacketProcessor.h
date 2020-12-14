#pragma once

#include "AlefLocalInfo.h"
#include "AlefCrypto.h"

class AlefPacketProcessor
{
public:
	AlefPacketProcessor() {};
	AlefPacketProcessor(AlefPacket* packet) { /*processPacket(packet);*/ };
	virtual ~AlefPacketProcessor() {};

	//virtual bool processPacket(AlefSocket& sock, AlefPacket* packet) { return true; };
	virtual bool processPacket(const localInfo& info) { return true; };
};