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
	bool processHashKeyPacket(AlefSocket& sock, AlefPacket* packet);
	bool processUserLoginPacket(AlefSocket& sock, AlefPacket* packet, char* acct, UInt8 acctLen, char* pw, UInt8 pwLen);
	bool processUnionInfo(AlefSocket& sock, AlefPacket* packet);
	bool processCharacterList(AlefSocket& sock, AlefPacket* packet);
	bool processCharacterCreation(AlefSocket& sock, AlefPacket* packet);
	bool processWorldEnterRequest(AlefSocket& sock, AlefPacket* packet);

	enum CLIENTLOGINOPERATION
	{
		CLIENTLOGIN_HASHKEY,
		CLIENTLOGIN_USERLOGIN,
	};
	
private:
	void sendDummyCharacter(AlefSocket& sock);
	void sendLoginResult(AlefSocket& sock, int loginResult);

	//AlefClientAccount clientAccount;

};