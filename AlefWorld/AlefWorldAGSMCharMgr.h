#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"

class AlefWorldAGSMCharMgr : public AlefPacketProcessor
{
public:
	AlefWorldAGSMCharMgr() {};
	virtual ~AlefWorldAGSMCharMgr() {};

	virtual bool processPacket(const localInfo& local);
	bool processGameEnterCharacterName(localInfo& local);
	bool processEnterWorld(localInfo& local);

	void sendDummyCharacter(AlefSocket& sock);
	
};