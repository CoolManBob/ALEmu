#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefLoginGlobal.h"
#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefLoginServerList : public AlefPacketProcessor
{
public:
	AlefLoginServerList() {};
	virtual ~AlefLoginServerList() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processServerList(AlefSocket& sock, AlefPacket* packet);

	enum SERVERLISTOPERATION
	{
		SERVERLIST_REQ = 0,
		SERVERLIST_ANS = 2,
	};
};