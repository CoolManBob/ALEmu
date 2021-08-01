#pragma once
#include "AlefTypes.h"
#include "AlefFlagLengthLookup.h"
#include "AlefFieldLookup.h"
#include "AlefPacketPool.h"

#include <cstdarg>

using std::string;

//Main interface for interacting with incoming/outgoing packets
class AlefPacketInterface 
{
public:
	AlefPacketInterface();
	~AlefPacketInterface();

	AlefPacket * buildPacket(UInt16 packetType, ...);
	AlefPacket * buildMiniPacket(UInt16 miniType, ...);

	bool processPacket(AlefPacket * packet, ...);
	bool setupPkt(AlefPacket * packet); //Sets PacketType, FlagLength, and Field information for incoming packets.
private:
	AlefFlagLengthLookup flagLookup;
	AlefFieldLookup fieldLookup;
};