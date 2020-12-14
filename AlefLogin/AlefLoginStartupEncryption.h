#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefLoginGlobal.h"
#include "AlefPacketProcessor.h"
#include "AlefCrypto.h"
#include "AlefLoginServerSystems.h"

class AlefLoginStartupEncryption : public AlefPacketProcessor
{
public:
	AlefLoginStartupEncryption() {};
	virtual ~AlefLoginStartupEncryption() {};

	virtual bool processPacket(const localInfo& local);
	bool processInitialPacket(localInfo& local);
	bool processCryptoPacket(localInfo& local);

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