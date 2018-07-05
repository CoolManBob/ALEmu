#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefLoginStartupEncryption::processPacket(AlefSocket& sock, AlefPacket * packet/*, blowfish_session& session*/)
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
	//response.WriteHeader(0x1A, 0x48, 0x03, 0x06);
	response.WriteInt8(0x08);
	response.WriteInt64(0);
	response.WriteInt8(0);
	response.WriteInt8(0x6B); //Footer

	//sock.sendBytes(response.getBuffer(), response.getSize());
	sock.sendPacket(&response, false);

	unsigned char serverKey[] = "11111111111111111111111111111111";
	//unsigned char serverKey[0x20] = "1111111111111111111111111111111";
	AlefPacket keyResponse(0x32, 0x48, 0x03, 0x01);
	keyResponse.WriteInt16(0x20);

	//blowfish_setkey(sock.getCryptoSession()->serverCtx, (unsigned char*)"11111111111111111111111111111111", (0x20 << 3));
	blowfish_setkey(sock.getCryptoSession()->serverCtx, serverKey, (0x20 << 3));
	for (UInt32 i = 0; i < (0x20 >> 3); i++) {
		blowfish_encrypt(sock.getCryptoSession()->serverCtx, (UInt32*)(serverKey + i * 8), (UInt32*)(serverKey + i * 8 + 4));
	}
	keyResponse.WriteByteArray(serverKey);
	keyResponse.WriteInt8(0x6B);

	//sock.sendBytes(keyResponse.getBuffer(), keyResponse.getSize());
	sock.sendPacket(&keyResponse, false);
	return true;
}

bool AlefLoginStartupEncryption::processCryptoPacket(AlefSocket& sock, AlefPacket * packet)
{
	cout << "processCryptoPacket" << endl;

	uint32_t keySize = (packet->getBuffer()[0x0F]) + (packet->getBuffer()[0x10] << 4);
	//unsigned char clientKey[0x21];
	unsigned char* clientKey = new unsigned char[keySize];
	memcpy(clientKey, packet->getBufferAt(0x11), keySize);
	/*for (unsigned int i = 0; i < 4; i++) 
	{
		uint32_t xl = 0, xr = 0;
		ByteToLong(&clientKey[i * 8], &xl, &xr);
		blowfish_decrypt(&cryptoSession.serverCtx, &xl, &xr);
		LongToByte(&clientKey[i * 8], &xl, &xr);
	}*/

	
	//keySize <<= 0x03;
	//blowfish_setkey(&cryptoSession.clientCtx, clientKey, 0x20);
	for (UInt32 i = 0; i < (keySize >> 3); i++) {
		blowfish_decrypt(sock.getCryptoSession()->serverCtx, (UInt32*)(clientKey + i * 8), (UInt32*)(clientKey + i * 8 + 4));
	}
	blowfish_setkey(sock.getCryptoSession()->clientCtx, clientKey, (keySize << 3));
	delete[] clientKey;

	AlefPacket * startupCryptoComplete = new AlefPacket(0x10, 0x48, 0x01, 0x03);
	startupCryptoComplete->WriteInt8(0x6B); //Footer

	sock.sendPacket(startupCryptoComplete, false);
	delete startupCryptoComplete;
	
	/*uint32_t rawPacketSize = (startupCryptoComplete->getSingleElement(0x01)) + (startupCryptoComplete->getSingleElement(0x02) << 4);
	uint32_t packetSize = rawPacketSize + 0x08;
	AlefPacket * encryptedStartupComplete = new AlefPacket(packetSize);
	encryptPacket(startupCryptoComplete, encryptedStartupComplete);
	sock.sendBytes(encryptedStartupComplete->getBuffer(), encryptedStartupComplete->getSize());

	delete startupCryptoComplete;*/
	//delete encryptedStartupComplete;

	return true;
}