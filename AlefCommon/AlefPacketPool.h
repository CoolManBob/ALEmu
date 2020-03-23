#pragma once

#include <Poco/Foundation.h>
#include <Poco/SharedPtr.h>

using Poco::SharedPtr;

#include "AlefPool.h"
#include "AlefPacket.h"


class AlefPacketPool
{
public:
	AlefPacketPool();
	AlefPacketPool(int accessThreshold);
	~AlefPacketPool();

	void addPacketToPool(AlefPacket* pkt);
	int	getSize() { return packetPool->size(); }

private:
	UInt64 packetID;
	AlefPool<int, SharedPtr<AlefPacket>> * packetPool;
};