#pragma once

#include "Poco/Net/StreamSocket.h"

using namespace Poco::Net;

#include "AlefSocketImpl.h"
#include "AlefPacket.h"

class AlefSocket : public StreamSocket
{
public:
	AlefSocket() {};
	AlefSocket(const Socket& sock) : StreamSocket(sock) {};
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
		/*byte	header[] = { 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		u32	id = 0;
		byte	*data = p->data;
		u32	size = p->size;

		if (data[0] != 0xD6 || size < 16 || size >(p->mem - 8)) {
			return;
		}

		if (size % 8) {
			u32 filler = 8 - (size % 8);
			size += filler;
			p->write_ptr += filler;
		}

		for (u32 i = 0; i < size >> 3; i++) {
			blowfish_encrypt(bf->server_context, (u32*)(data + i * 8), (u32*)(data + i * 8 + 4));
		}
		memmove(data + 7, data, size);
		memcpy(data, header, 7);
		data[size + 7] = 0xAF;
		p->write_ptr += 8;
		*(u32*)(data + 3) = ++bf->packet_id;
		adjustPacketSize(p);*/

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
				blowfish_encrypt(cryptoSession->serverCtx, (UInt32*)(packetData + i * 8), (UInt32*)(packetData + i * 8 + 4));

			memmove(packetData + 7, packetData, packetSize);
			memcpy(packetData, header, 7);
			packetData[packetSize + 7] = 0xAF;
			*(UInt32*)(packetData + 3) = ++cryptoSession->packetCounter;
			
			return this->sendBytes(packet->getBuffer(), packet->getSize());
		}
		else
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