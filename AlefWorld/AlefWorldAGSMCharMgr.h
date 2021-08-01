#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefWorldGlobal.h"
#include "AlefWorldServerSystems.h"

class AlefWorldAGSMCharMgr : public AlefPacketProcessor
{
public:
	AlefWorldAGSMCharMgr() {};
	virtual ~AlefWorldAGSMCharMgr() {};

	virtual bool processPacket(const localInfo& local);
	bool processGameEnterCharacterName(localInfo& local, string charName, Int32 authToken);
	bool processEnterWorld(localInfo& local);

	void sendDummyCharacter(AlefSocket& sock);
	
};