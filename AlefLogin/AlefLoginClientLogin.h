#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"

class AlefLoginClientLogin : public AlefPacketProcessor
{
public:
	AlefLoginClientLogin();
	virtual ~AlefLoginClientLogin();

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processInitialLoginPacket(AlefSocket& sock, AlefPacket* packet);
	bool processUserLoginPacket(AlefSocket& sock, AlefPacket* packet, unsigned char* acct, UInt8 acctLen, unsigned char* pw, UInt8 pwLen);
	bool processUnionInfo(AlefSocket& sock, AlefPacket* packet);
	bool processCharacterList(AlefSocket& sock, AlefPacket* packet);
	bool processCharacterCreation(AlefSocket& sock, AlefPacket* packet);
	bool processWorldEnterRequest(AlefSocket& sock, AlefPacket* packet);
	
private:
	void sendDummyCharacter(AlefSocket& sock);

	//AlefClientAccount clientAccount;

};