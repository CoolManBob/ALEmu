#include "AlefPacketPool.h"

AlefPacketPool::AlefPacketPool()
{
	packetID = 0;
	packetPool = new AlefPool<int, SharedPtr<AlefPacket>>();
}

AlefPacketPool::AlefPacketPool(int accessThreshold)
{
	packetID = 0;
	packetPool = new AlefPool<int, SharedPtr<AlefPacket>>(accessThreshold);
}

AlefPacketPool::~AlefPacketPool()
{
	if (packetPool)
	{
		packetPool->clear();
		delete packetPool;
	}
}

void AlefPacketPool::addPacketToPool(AlefPacket* pkt)
{
	SharedPtr<AlefPacket> sharedPkt(pkt);
	packetPool->add(packetID, sharedPkt);
	packetID++; //Since this is unsigned it doesn't matter if we let this eventually roll over (pro tip: its 64bits so it realistically will not.)
}