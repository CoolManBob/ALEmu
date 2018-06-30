#pragma once

#include "AlefPacket.h"
#include "AlefCrypto.h"

class AlefPacketProcessor
{
public:
	AlefPacketProcessor() {};
	AlefPacketProcessor(AlefPacket* packet) { /*processPacket(packet);*/ };
	virtual ~AlefPacketProcessor() {};

	virtual bool processPacket(StreamSocket& sock, AlefPacket* packet, blowfish_session& session) { return true; };
};