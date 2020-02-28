#pragma once

#include "Poco/Net/StreamSocket.h"

using namespace Poco::Net;

#include "AlefSocketImpl.h"
#include "AlefPacket.h"

class AlefSocket : public StreamSocket
{
public:
	AlefSocket() { cryptoSession = nullptr; };
	AlefSocket(const Socket& sock) : StreamSocket(sock) { cryptoSession = nullptr; };
	virtual ~AlefSocket() {};

	AlefSocket& operator = (const Socket& sock) 
	{ 
		if (dynamic_cast<AlefSocketImpl*>(sock.impl()))
			Socket::operator = (sock);
	}

	void setCryptoSession(blowfish_session* session) { cryptoSession = session; }
	blowfish_session* getCryptoSession() { return cryptoSession; }
	int sendPacket(AlefPacket* packet, bool encrypt = true) 
	{
		if (encrypt)
		{
			UInt8 header[] = { 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			UInt8 * packetData = packet->getBuffer();
			UInt32 packetSize = packet->getSize();

			if (packetData[0] != 0xD6 || packetSize < 16)
				return 0;
			if (packetSize % 8)
			{
				UInt32 padding = 8 - (packetSize % 8);
				packetSize += padding;
			}

			packet->Resize(packetSize);

			for (UInt32 i = 0; i < (packetSize >> 3); i++)
				blowfish_encrypt(cryptoSession->serverCtx, (UInt32*)(packet->getBuffer() + i * 8), (UInt32*)(packet->getBuffer() + i * 8 + 4));

			packet->Resize(packetSize + 8);
			memmove(packet->getBuffer() + 7, packet->getBuffer(), packetSize);
			memcpy(packet->getBuffer(), header, 7);
			packet->getBuffer()[packetSize + 7] = 0xAF; //Guard Footer Byte
			*(UInt16*)(packet->getBuffer() + 1) = ((UInt16)packet->getSize()); //Encrypted Size
			*(UInt32*)(packet->getBuffer() + 3) = ++cryptoSession->packetCounter;
			
			//return this->sendBytes(packet->getBuffer(), packet->getSize());
		}
		
		return this->sendBytes(packet->getBuffer(), packet->getSize());
	}
	int receivePacket(AlefPacket * packet) 
	{
		/*unsigned char tempBuf[maxReceiveBytes + 1] = { 0 };

		int readBytes = receiveBytes(tempBuf, maxReceiveBytes);

		if (readBytes <= 0)
			return 0;

		packet = new AlefPacket(tempBuf, readBytes);

		UInt32 realSize = 0;
		UInt8* packetData = packet->getBuffer();
		int isServerPacket = (packetData[0] == 0xA1) ? 1 : 0;

		if (packetData[0] == 0xD6 && packet->getSize() >= 16)
			return readBytes;

		if ((packetData[0] != 0xB1 && packetData[0] != 0xA1) || packet->getSize() < 24)
			return false;

		packet->setSize(packet->getSize() - 8);
		memmove(packetData, packetData + 7, packet->getSize());

		for (int i = 0; i < (packet->getSize() >> 3); ++i)
			blowfish_decrypt((isServerPacket) ? cryptoSession->serverCtx : cryptoSession->clientCtx, (UInt32*)(packetData + i * 8), (UInt32*)(packetData + i * 8 + 4));

		if (packetData[0] != 0xD6)
			return false;

		return readBytes;*/
	}



private:
	blowfish_session * cryptoSession;
};