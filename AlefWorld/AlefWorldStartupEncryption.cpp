#include <iostream>
using std::cout;
using std::endl;

#include "AlefWorldGlobal.h"
#include "AlefWorldStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefWorldStartupEncryption::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	switch (packet->GetPacketFlag(FlagIndex::FLAG_IDX0))
	{
	case 1: //Initial Packet Opcode 0x48 0x01
		return processInitialPacket(sock, packet); break;
	case 3: //Crypto Packets Opcode 0x48 0x03
		return processCryptoPacket(sock, packet); break;
	default:
		return false;
	}
	return false;
}

bool AlefWorldStartupEncryption::processInitialPacket(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processInitialPacket");
	AlefPacket response(0x48, 0x03, 0x06);
	response.WriteInt8(0x08);
	response.WriteInt64(0);
	response.WriteInt8(0);
	response.ClosePacket();
	//response.WriteInt8(0x6B); //Footer

	sock.sendPacket(&response, false);

	unsigned char serverKey[] = "12345678123456781234567812345678";

	AlefPacket keyResponse(0x48, 0x03, 0x01);
	keyResponse.WriteInt16(0x20);

	blowfish_setkey(sock.getCryptoSession()->serverCtx, serverKey, (0x20 << 3));

	keyResponse.WriteByteArray(serverKey);
	keyResponse.ClosePacket();
	//keyResponse.WriteInt8(0x6B);

	sock.sendPacket(&keyResponse, false);
	return true;
}

bool AlefWorldStartupEncryption::processCryptoPacket(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processCryptoPacket");

	uint32_t keySize = (packet->getBuffer()[0x0F]) + (packet->getBuffer()[0x10] << 4);
	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, packet->getBufferAt(0x11), keySize);

	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(sock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * 8), (UInt32*)(clientKey + i * 8 + 4));
	}
	blowfish_setkey(sock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	AlefPacket startupCryptoComplete(0x48, 0x01, 0x03);
	startupCryptoComplete.ClosePacket();
	//startupCryptoComplete.WriteUInt8(0x6B); //Footer
	sock.sendPacket(&startupCryptoComplete);

	return true;
}