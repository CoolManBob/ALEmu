#pragma once

#include "Poco/Net/StreamSocket.h"
using Poco::Net::StreamSocket;

#include "AlefPacketProcessor.h"

class AlefLoginStartupEncryption : public AlefPacketProcessor
{
public:
	AlefLoginStartupEncryption() {};
	virtual ~AlefLoginStartupEncryption() {};

	bool encryptPacket(AlefPacket * decryptedPacket, AlefPacket * encryptedPacket)
	{
		unsigned char * rawPacket = new unsigned char[decryptedPacket->getSize()];
		memcpy(rawPacket, decryptedPacket->getBuffer(), decryptedPacket->getSize());
		uint32_t rawPacketSize = (rawPacket[0x01]) + (rawPacket[0x02] << 4);
		uint32_t packetSize = rawPacketSize + 0x08;
		for (unsigned int i = 0; i < (rawPacketSize >> 0x03); i++) 
		{
			uint32_t xl = 0, xr = 0;
			ByteToLong(&rawPacket[i * 8], &xl, &xr);
			blowfish_encrypt(&cryptoSession.serverCtx, &xl, &xr);
			LongToByte(&rawPacket[i * 8], &xl, &xr);	
		}
		//encryptedPacket->Resize((packetSize & 0xFF) + (LOWORD(packetSize) >> 0x08));
		encryptedPacket[0] = 0xa1;
		encryptedPacket[1] = (packetSize & 0xFF); 
		encryptedPacket[2] = (LOWORD(packetSize) >> 0x08);
		memcpy(&encryptedPacket[3], &rawPacket[0], rawPacketSize);
		encryptedPacket[packetSize - 1] = 0xaf;

		return true;
	}

	virtual bool processPacket(StreamSocket& sock, AlefPacket* packet, blowfish_session& session);
	bool processInitialPacket(StreamSocket& sock, AlefPacket * packet);
	bool processCryptoPacket(StreamSocket& sock, AlefPacket * packet);

private:
	blowfish_session cryptoSession;
};