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

	virtual bool processPacket(const localInfo& local);
	bool processServerList(localInfo& local);

	enum SERVERLISTOPERATION
	{
		SERVERLIST_REQ = 0,
		SERVERLIST_ANS = 2,
	};
};