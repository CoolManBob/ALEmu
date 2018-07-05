#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefLoginStartupEncryption::processPacket(AlefSocket& sock, AlefPacket * packet)
{
	switch (packet->GetPacketHeader(true).PacketFlag)
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

bool AlefLoginStartupEncryption::processInitialPacket(AlefSocket& sock, AlefPacket * packet)
{
	cout << "processInitialPacket" << endl;
	AlefPacket response(0x1A, 0x48, 0x03, 0x06);
	response.WriteInt8(0x08);
	response.WriteInt64(0);
	response.WriteInt8(0);
	response.WriteInt8(0x6B); //Footer

	sock.sendPacket(&response, false);

	unsigned char serverKey[] = "11111111111111111111111111111111";
	AlefPacket keyResponse(0x32, 0x48, 0x03, 0x01);
	keyResponse.WriteInt16(0x20);

	blowfish_setkey(sock.getCryptoSession()->serverCtx, serverKey, (0x20 << 3));

	keyResponse.WriteByteArray(serverKey);
	keyResponse.WriteInt8(0x6B);

	sock.sendPacket(&keyResponse, false);
	return true;
}

bool AlefLoginStartupEncryption::processCryptoPacket(AlefSocket& sock, AlefPacket * packet)
{
	cout << "processCryptoPacket" << endl;

	uint32_t keySize = (packet->getBuffer()[0x0F]) + (packet->getBuffer()[0x10] << 4);
	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, packet->getBufferAt(0x11), keySize);

	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(sock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * 8), (UInt32*)(clientKey + i * 8 + 4));
	}
	blowfish_setkey(sock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	AlefPacket * startupCryptoComplete = new AlefPacket(0x10, 0x48, 0x01, 0x03);
	startupCryptoComplete->WriteInt8(0x6B); //Footer

	sock.sendPacket(startupCryptoComplete, false);
	delete startupCryptoComplete;

	return true;
}