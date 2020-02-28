#pragma once
#include "AlefTypes.h"
#include "AlefPacket.h"
#include "AlefFlagLengthLookup.h"
#include "AlefFieldLookup.h"

#include <cstdarg>

//Main interface for interacting with incoming/outgoing packets
class AlefPacketInterface
{
public:
	AlefPacketInterface();
	~AlefPacketInterface();

	AlefPacket * buildPacket(UInt16 packetType, UInt8 flagLength, ...);
	AlefPacket * buildMiniPacket(UInt16 miniType, UInt8 flagLength, ...);

	bool processPacket(AlefPacket * packet, ...);
	bool setupPkt(AlefPacket * packet); //Sets PacketType, FlagLength, and Field information for incoming packets.
private:
	AlefFlagLengthLookup flagLookup;
	AlefFieldLookup fieldLookup;
};