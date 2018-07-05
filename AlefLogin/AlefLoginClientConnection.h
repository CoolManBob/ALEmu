#pragma once
#pragma pack(push,1)
#include <iostream>
using std::cout;
using std::endl;

#include "Poco/Exception.h"
using Poco::Exception;

#include "AlefServerConnection.h"
#include "AlefLoginPacketHandler.h"
#include "AlefSocket.h"
#include "AlefCrypto.h"

const int maxReceiveBytes = 4096;

class AlefLoginClientConnection : public AlefServerConnection
{
public:
	AlefLoginClientConnection(const AlefSocket& socket, AlefLoginPacketHandler* packetHandler) : AlefServerConnection(socket, handler), handler(packetHandler), sock(socket)
	{
		cryptSession = new blowfish_session;
		cryptSession->serverCtx = new blowfish_context;
		cryptSession->clientCtx = new blowfish_context;
		cryptSession->packetCounter = 0;
		blowfish_init(cryptSession->serverCtx);
		blowfish_init(cryptSession->clientCtx);
		sock.setCryptoSession(cryptSession);
	};
	virtual ~AlefLoginClientConnection() 
	{
		delete cryptSession->serverCtx;
		delete cryptSession->clientCtx;
		delete cryptSession;
		cout << "Client disconnected" << endl;
	};

	virtual void run()
	{
		unsigned char tempBuf[maxReceiveBytes + 1] = { 0 };
		int numBytesRead = 1;
		for (;numBytesRead;)
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
						cout << "ERROR Decrypting Packet" << endl;
						continue;
					}
					packetInfo info(sock);
					AlefPacketHeader header;
					header = packet->GetPacketHeader(true);
					info.PacketType = header.PacketType;
					info.PacketFlag = header.PacketFlag;
					info.PacketOperation = header.PacketOperation;
					info.packet = packet;
					cout << "numBytesRead: " << numBytesRead << endl;
					cout << "PacketSize: " << header.PacketSize << endl;
					cout << "Opcode " << (int)info.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << endl;
					ActiveResult<bool> res = handler->packetHandler(info);
					res.wait();
					if (res.data())
					{
						cout << "Opcode " << (int)header.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << " handled successfully." << endl;
						delete packet;
					}
					else
					{
						cout << "Error handling Opcode " << (int)header.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << endl;
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
			cout << "Unencrypted Packet" << endl;
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
	AlefLoginPacketHandler * handler;
	AlefSocket sock;
	blowfish_session * cryptSession;
};