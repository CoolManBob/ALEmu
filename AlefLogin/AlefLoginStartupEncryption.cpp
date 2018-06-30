#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginStartupEncryption.h"
#include "AlefCrypto.h"

bool AlefLoginStartupEncryption::processPacket(StreamSocket& sock, AlefPacket * packet, blowfish_session& session)
{
	cryptoSession = session;
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

bool AlefLoginStartupEncryption::processInitialPacket(StreamSocket& sock, AlefPacket * packet)
{
	cout << "processInitialPacket" << endl;
	AlefPacket response(0x1A, 0x48, 0x03, 0x06);
	//response.WriteHeader(0x1A, 0x48, 0x03, 0x06);
	response.WriteInt8(0x08);
	response.WriteInt64(0);
	response.WriteInt8(0);
	response.WriteInt8(0x6B); //Footer

	sock.sendBytes(response.getBuffer(), response.getSize());

	unsigned char serverKey[] = "11111111111111111111111111111111";
	AlefPacket keyResponse(0x32, 0x48, 0x03, 0x01);
	keyResponse.WriteInt16(0x20);

	blowfish_setkey(&cryptoSession.serverCtx, (unsigned char*)"11111111111111111111111111111111", 0x20);
	/*for (unsigned int i = 0; i < (0x20 >> 0x03); i++)
	{
		uint32_t xl = 0, xr = 0;
		ByteToLong(&serverKey[i*8], &xl, &xr);
		blowfish_encrypt(&cryptoSession.serverCtx, &xl, &xr);
		LongToByte(&serverKey[i*8], &xl, &xr);
	}*/
	keyResponse.WriteByteArray(serverKey);
	keyResponse.WriteInt8(0x6B);

	sock.sendBytes(keyResponse.getBuffer(), keyResponse.getSize());
	return true;
}

bool AlefLoginStartupEncryption::processCryptoPacket(StreamSocket& sock, AlefPacket * packet)
{
	cout << "processCryptoPacket" << endl;

	unsigned char clientKey[0x20];
	memcpy(&clientKey, packet->getBufferAt(0x11), 0x20);
	for (unsigned int i = 0; i < 4; i++) 
	{
		uint32_t xl = 0, xr = 0;
		ByteToLong(&clientKey[i * 8], &xl, &xr);
		blowfish_decrypt(&cryptoSession.serverCtx, &xl, &xr);
		LongToByte(&clientKey[i * 8], &xl, &xr);
	}

	/*uint32_t keySize = (packet->getBuffer()[0x0F]) + (packet->getBuffer()[0x10] << 4);
	keySize <<= 0x03;*/
	blowfish_setkey(&cryptoSession.clientCtx, clientKey, 0x20);

	AlefPacket * startupCryptoComplete = new AlefPacket(0x10, 0x48, 0x01, 0x03);
	startupCryptoComplete->WriteInt8(0x6B); //Footer
	
	uint32_t rawPacketSize = (startupCryptoComplete->getSingleElement(0x01)) + (startupCryptoComplete->getSingleElement(0x02) << 4);
	uint32_t packetSize = rawPacketSize + 0x08;
	AlefPacket * encryptedStartupComplete = new AlefPacket(packetSize);
	encryptPacket(startupCryptoComplete, encryptedStartupComplete);
	sock.sendBytes(encryptedStartupComplete->getBuffer(), encryptedStartupComplete->getSize());

	delete startupCryptoComplete;
	//delete encryptedStartupComplete;

	return true;
}