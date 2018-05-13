#pragma once

#include "AlefPacket.h"

class AlefPacketProcessor
{
public:
	AlefPacketProcessor() {};
	AlefPacketProcessor(AlefPacket* packet) { processPacket(packet); };
	virtual ~AlefPacketProcessor() {};

	virtual bool processPacket(AlefPacket* packet) {};
};