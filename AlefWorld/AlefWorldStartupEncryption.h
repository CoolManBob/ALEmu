#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"
#include "AlefSocket.h"
#include "AlefWorldGlobal.h"
#include "AlefCrypto.h"
#include "AlefWorldServerSystems.h"

class AlefWorldStartupEncryption : public AlefPacketProcessor
{
public:
	AlefWorldStartupEncryption() {};
	virtual ~AlefWorldStartupEncryption() {};

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