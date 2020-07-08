#include <iostream>
using std::cout;
using std::endl;

#include "AlefWorldGlobal.h"
#include "AlefWorldStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefWorldStartupEncryption::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	//Parse Packet here and then handle opcode
	int operation = 0;
	int nSize = 0;
	char* data = nullptr;
	/*Alef::INT8, Alef::MEMORY_BLOCK*/
	/*1, 1*/
	pktInterface->processPacket(packet, &operation, data);

	switch (operation)
	{
	case 0: //Initial Packet Opcode 0x48 0x01
		return processInitialPacket(sock, packet); break;
	case 2: //Crypto Packet Opcode 0x48 0x03
		return processCryptoPacket(sock, packet); break;
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

bool AlefWorldStartupEncryption::processInitialPacket(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processInitialPacket");

	Int8 i8Operation = 6;
	UInt16 algoType = 0;

	SharedPtr<AlefPacket> response = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &algoType, 0);

	if (response)
		sock.sendPacket(response, false);

	unsigned char serverKey[] = "12345678123456781234567812345678";

	i8Operation = 1;
	UInt16 u16Sz = 32;

	SharedPtr<AlefPacket> keyResponse = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, &u16Sz, serverKey);

	blowfish_setkey(sock.getCryptoSession()->serverCtx, serverKey, (0x20 << 3));

	if (keyResponse)
		sock.sendPacket(keyResponse, false);

	return true;
}

bool AlefWorldStartupEncryption::processCryptoPacket(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processCryptoPacket");

	Int8 i8Operation = 3;

	uint32_t keySize = (packet->getBuffer()[0x0F]) + (packet->getBuffer()[0x10] << 4);
	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, packet->getBufferAt(0x11), keySize);

	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(sock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * (__int64)8), (UInt32*)(clientKey + i * (__int64)8 + (__int64)4));
	}
	blowfish_setkey(sock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	SharedPtr<AlefPacket> startupCryptoComplete = pktInterface->buildPacket(Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE, &i8Operation, 0, 0);
	sock.sendPacket(startupCryptoComplete);

	return true;
}