#include <iostream>
using std::cout;
using std::endl;

#include "AlefWorldGlobal.h"
#include "AlefWorldStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefWorldStartupEncryption::processPacket(const localInfo& local)
{
	//Parse Packet here and then handle opcode
	//AuPacket::GetField(&v6->m_csPacket, 1, v7, v8, &pvPacket, &pCode, &nSize);
	localInfo& localObj = const_cast<localInfo&>(local);
	AlefPacket* packet = localObj.packet;
	int operation = 0;
	int nSize = 0;
	char* data = nullptr;
	/*Alef::INT8, Alef::MEMORY_BLOCK*/
	/*1, 1*/
	pktInterface->processPacket(packet, &operation, data);

	switch (operation)
	{
	case 0: //Initial Packet Opcode 0x48 0x01
		return processInitialPacket(localObj); break;
	case 2: //Crypto Packet Opcode 0x48 0x03
		return processCryptoPacket(localObj); break;
	default:
	{
		if (data)
			delete[] data;
			return false;
	}
	}
	if (data)
		delete[] data;
	return false;
}

bool AlefWorldStartupEncryption::processInitialPacket(localInfo& local)
{
	LOG("processInitialPacket");

	Int8 i8Operation = 6;
	UInt16 algoType = 0;

	SharedPtr<AlefPacket> response = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &algoType, 0);

	if (response)
		_localSock.sendPacket(response, false);

	unsigned char serverKey[] = "12345678123456781234567812345678";

	i8Operation = 1;
	UInt16 u16Sz = 32;

	SharedPtr<AlefPacket> keyResponse = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &u16Sz, serverKey);

	blowfish_setkey(_localSock.getCryptoSession()->serverCtx, serverKey, (0x20 << 3));

	if (keyResponse)
		_localSock.sendPacket(keyResponse, false);

	return true;
}

bool AlefWorldStartupEncryption::processCryptoPacket(localInfo& local)
{
	LOG("processCryptoPacket");

	Int8 i8Operation = 3;

	uint32_t keySize = (_localPkt->getBuffer()[0x0F]) + (_localPkt->getBuffer()[0x10] << 4);
	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, _localPkt->getBufferAt(0x11), keySize);

	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(_localSock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * (__int64)8), (UInt32*)(clientKey + i * (__int64)8 + (__int64)4));
	}
	blowfish_setkey(_localSock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	SharedPtr<AlefPacket> startupCryptoComplete = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, 0, 0);
	_localSock.sendPacket(startupCryptoComplete);

	return true;
}