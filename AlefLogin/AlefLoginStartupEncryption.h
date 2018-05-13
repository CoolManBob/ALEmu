#pragma once

#include "AlefPacketProcessor.h"

class AlefLoginStartupEncryption : public AlefPacketProcessor
{
public:
	AlefLoginStartupEncryption(AlefPacket * packet) {};
	virtual ~AlefLoginStartupEncryption() {};

	virtual bool processPacket(AlefPacket* packet) {};
};