#pragma once
#pragma pack(push,1)
#include <iostream>
using std::cout;
using std::endl;

#include "Poco/Exception.h"
using Poco::Exception;

#include "AlefServerConnection.h"
#include "AlefLoginPacketHandler.h"
#include "AlefCrypto.h"

const int maxReceiveBytes = 2048;

class AlefLoginClientConnection : public AlefServerConnection
{
public:
	//AlefLoginClientConnection() {};
	AlefLoginClientConnection(const StreamSocket& socket, AlefLoginPacketHandler* packetHandler) : AlefServerConnection(socket, handler), handler(packetHandler)
	{
		blowfish_init(&session.serverCtx);
		blowfish_init(&session.clientCtx);
	};
	virtual ~AlefLoginClientConnection() 
	{
		blowfish_free(&session.serverCtx);
		blowfish_free(&session.clientCtx);
	};

	virtual void run()
	{
		//cout << "AlefLoginClientConnection Spawned...Now waiting data..." << endl;
		char tempBuf[maxReceiveBytes + 1] = { 0 };
		int numBytesRead = 1;
		for (;numBytesRead;)
		{
			try
			{
				numBytesRead = socket().receiveBytes(tempBuf, maxReceiveBytes);
				if (numBytesRead)
				{
					AlefPacket * packet = new AlefPacket(tempBuf, numBytesRead);
					packetInfo info(socket(), session);
					AlefPacketHeader header;
					header = packet->GetPacketHeader(true);
					info.PacketType = header.PacketType;//packet->GetPacketHeader(true).PacketType;
					info.PacketFlag = header.PacketFlag;
					info.PacketOperation = header.PacketOperation;
					info.packet = packet;
					cout << "numBytesRead: " << numBytesRead << endl;
					cout << "PacketSize: " << header.PacketSize << endl;
					cout << "Opcode " << (int)info.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << endl;
					ActiveResult<bool> res = handler->packetHandler(info);
					res.wait();
					if (res.data())
						cout << "Opcode " << (int)header.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << " handled successfully." << endl;
					else
						cout << "Error handling Opcode " << (int)header.PacketType << " " << (int)header.PacketFlag << " " << (int)header.PacketOperation << endl;
				} 

			}
			catch (Exception&)
			{

			}
		}
	}
private:
	AlefLoginPacketHandler * handler;
	blowfish_session session;
};