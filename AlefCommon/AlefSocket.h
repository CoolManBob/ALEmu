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
	};

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
			return 0;
		}
		else
			return this->sendBytes(packet->getBuffer(), packet->getSize());
	};

private:
	blowfish_session * cryptoSession;
};