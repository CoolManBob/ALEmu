#include <iostream>
using std::cout;
using std::endl;

#include "AlefWorldStartupEncryption.h"

bool AlefWorldStartupEncryption::processPacket(const localInfo& local)
{
	//AuPacket::GetField(&v6->m_csPacket, 1, v7, v8, &pvPacket, &pCode, &nSize);
	localInfo& localObj = const_cast<localInfo&>(local);
	AlefPacket* packet = localObj.packet;
	Int8 i8Operation = 0;
	/*Alef::INT8, Alef::MEMORY_BLOCK*/
	/*1, 1*/
	pktInterface->processPacket(packet, &i8Operation, 0);

	switch (i8Operation)
	{
	case CRYPT_REQ: //Client Key Request
		return processInitialPacket(localObj); break;
	case CRYPT_PRIVATEKEY: //Key Response - Client Blowfish Private Key
		return processCryptoPacket(localObj); break;
	default:
	{
		return false;
	}
	}
	return false;
}

bool AlefWorldStartupEncryption::processInitialPacket(localInfo& local)
{
	LOG("processInitialPacket");

	Int8 i8Operation = CRYPT_SETALGO;
	UInt16 algoType = CRYPT_ALGO_BLOWFISH;

	SharedPtr<AlefPacket> response = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &algoType, 0);

	if (response)
		_localSock.sendPacket(response, false);

	std::string serverKey;
	bool autoGen = worldConfig->getAutoGenKey();
	if (autoGen)
		serverKey = serverEncryptionSys->getCryptKey();
	else
		serverKey = worldConfig->getCryptKey();

	i8Operation = CRYPT_PUBLICKEY;
	UInt16 u16Sz = 32;

	SharedPtr<AlefPacket> keyResponse = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &u16Sz, &serverKey);

	blowfish_setkey(_localSock.getCryptoSession()->serverCtx, (unsigned char*)serverKey.c_str(), (0x20 << 3));

	_localSock.sendPacket(keyResponse, false);

	return true;
}

bool AlefWorldStartupEncryption::processCryptoPacket(localInfo& local)
{
	LOG("processCryptoPacket");

	uint32_t keySize = (_localPkt->getBuffer()[0x0F]) + (_localPkt->getBuffer()[0x10] << 4);

	if (keySize > 32 || keySize < 32)
		return false;

	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, _localPkt->getBufferAt(0x11), keySize);

	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(_localSock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * (__int64)8), (UInt32*)(clientKey + i * (__int64)8 + (__int64)4));
	}
	blowfish_setkey(_localSock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	Int8 i8Operation = CRYPT_COMPLETE;

	SharedPtr<AlefPacket> startupCryptoComplete = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, 0, 0);
	_localSock.sendPacket(startupCryptoComplete);

	return true;
}