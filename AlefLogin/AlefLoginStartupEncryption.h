#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefLoginGlobal.h"
#include "AlefPacketProcessor.h"
#include "AlefSocket.h"
#include "AlefPacketInterface.h"
#include "AlefCrypto.h"

class AlefLoginStartupEncryption : public AlefPacketProcessor
{
public:
	AlefLoginStartupEncryption() {};
	virtual ~AlefLoginStartupEncryption() {};

	virtual bool processPacket(AlefSocket& sock, AlefPacket* packet);
	bool processInitialPacket(AlefSocket& sock, AlefPacket * packet);
	bool processCryptoPacket(AlefSocket& sock, AlefPacket * packet);

	enum CRYPTOPERATION : Int8
	{
		CRYPT_REQ = 0,
		CRYPT_PUBLICKEY,
		CRYPT_PRIVATEKEY,
		CRYPT_COMPLETE,
		CRYPT_DYNCODE_PUB,
		CRYPT_DYNCODE_PRV,
		CRYPT_SETALGO,
	};

	//We only support blowfish algo
	enum CRYPTALGO : Int16
	{
		CRYPT_ALGO_BLOWFISH = 0,
	};
};