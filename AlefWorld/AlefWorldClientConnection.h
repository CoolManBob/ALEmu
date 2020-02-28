#pragma once
//#pragma pack(push,1)
#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

#include "Poco/Exception.h"
using Poco::Exception;

#include "AlefWorldGlobal.h"
#include "AlefServerConnection.h"
#include "AlefWorldPacketHandler.h"
#include "AlefSocket.h"
#include "AlefCrypto.h"
#include "AlefFlagLengthLookup.h"

const int maxReceiveBytes = 4096;

class AlefWorldClientConnection : public AlefServerConnection
{
public:
	AlefWorldClientConnection(const AlefSocket& socket, AlefWorldPacketHandler* packetHandler) : AlefServerConnection(socket, handler), handler(packetHandler), sock(socket)
	{
		cryptSession = new blowfish_session;
		cryptSession->serverCtx = new blowfish_context;
		cryptSession->clientCtx = new blowfish_context;
		cryptSession->packetCounter = 0;
		blowfish_init(cryptSession->serverCtx);
		blowfish_init(cryptSession->clientCtx);
		sock.setCryptoSession(cryptSession);
	};
	virtual ~AlefWorldClientConnection()
	{
		delete cryptSession->serverCtx;
		delete cryptSession->clientCtx;
		delete cryptSession;
		LOG("Client disconnected");
	};

	virtual void run()
	{
		unsigned char tempBuf[maxReceiveBytes + 1] = { 0 };
		int numBytesRead = 1;
		for (; numBytesRead;)
		{
			try
			{
				numBytesRead = sock.receiveBytes(tempBuf, maxReceiveBytes);
				if (numBytesRead)
				{
					AlefPacket * packet = new AlefPacket(tempBuf, numBytesRead);
					bool decryptRes = decryptPacket(packet);
					if (!decryptRes)
					{
						LOG("ERROR Decrypting Packet");
						continue;
					}
					packetInfo info(sock);

					packet->acquirePacketHeader();
					UInt8 flagLen = lookup.lookUp(packet->GetPacketType());

					if (flagLen == 0xFF)
					{
						LOG("ERROR: Could not find FlagLength!", FATAL);
						delete packet;
						continue;
					}

					packet->setAndAcquireFlags(flagLen);
					info.packet = packet;

					stringstream outMsg;
					outMsg << "numBytesRead: " << numBytesRead << endl;
					outMsg << "PacketSize: " << packet->GetPacketSize() << endl;
					outMsg << "Opcode " << (int)packet->GetPacketType();// << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << endl;
					LOG(outMsg.str());
					ActiveResult<bool> res = handler->packetHandler(info);
					res.wait();
					if (res.data())
					{
						stringstream successMsg;
						successMsg << "Opcode " << (int)packet->GetPacketType() << " " << (int)packet->GetPacketFlag(FlagIndex::FLAG_IDX0) << " " << (int)packet->GetPacketFlag(FlagIndex::FLAG_IDX1) << " handled successfully.";
						LOG(successMsg.str());
						delete packet;
					}
					else
					{
						stringstream errorMsg;
						errorMsg << "Error handling Opcode " << (int)packet->GetPacketType() << " " << (int)packet->GetPacketFlag(FlagIndex::FLAG_IDX0) << " " << (int)packet->GetPacketFlag(FlagIndex::FLAG_IDX1);
						LOG(errorMsg.str(), FATAL);
						delete packet;
					}
				}

			}
			catch (Exception&)
			{

			}
		}
	}
private:
	bool decryptPacket(AlefPacket * packet)
	{
		UInt32 realSize = 0;
		UInt8* packetData = packet->getBuffer();
		int isServerPacket = (packetData[0] == 0xA1) ? 1 : 0;

		if (packetData[0] == 0xD6 && packet->getSize() >= 16)
		{
			LOG("Unencrypted Packet");
			return true;
		}

		if ((packetData[0] != 0xB1 && packetData[0] != 0xA1) || packet->getSize() < 24)
			return false;

		packet->setSize(packet->getSize() - 8);
		memmove(packetData, packetData + 7, packet->getSize());

		for (int i = 0; i < (packet->getSize() >> 3); ++i)
			blowfish_decrypt((isServerPacket) ? cryptSession->serverCtx : cryptSession->clientCtx, (UInt32*)(packetData + i * 8), (UInt32*)(packetData + i * 8 + 4));

		if (packetData[0] != 0xD6)
			return false;

		return true;
	}
	AlefFlagLengthLookup lookup;
	AlefWorldPacketHandler * handler;
	AlefSocket sock;
	blowfish_session * cryptSession;
};