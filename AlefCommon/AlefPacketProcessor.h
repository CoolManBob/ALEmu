#pragma once

#include "AlefPacket.h"
#include "AlefCrypto.h"
#include "AlefSocket.h"

class AlefPacketProcessor
{
public:
	AlefPacketProcessor() {};
	AlefPacketProcessor(AlefPacket* packet) { /*processPacket(packet);*/ };
	virtual ~AlefPacketProcessor() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet/*, blowfish_session& session*/) { return true; };
};