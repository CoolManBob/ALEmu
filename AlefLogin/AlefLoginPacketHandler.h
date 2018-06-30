#pragma once

#include "AlefPacketHandler.h"
#include "AlefLoginStartupEncryption.h"
#include "AlefPacketTypes.h"
#include "AlefCrypto.h"

class AlefLoginPacketHandler : public AlefPacketHandler
{
public:
	AlefLoginPacketHandler() : packetHandler(this, &AlefLoginPacketHandler::packetProcessor) 
	{
		processorMap[AGPMSTARTUPENCRYPTION_PACKET_TYPE] = new AlefLoginStartupEncryption();
	}
	virtual ~AlefLoginPacketHandler() {};

	bool decryptPacket(blowfish_session& session, AlefPacket* encryptedPacket, AlefPacket* decryptedPacket) 
	{ 
		uint32_t packetSize = (encryptedPacket->getSingleElement(0x01)) + (encryptedPacket->getSingleElement(0x02) << 4);
		
		unsigned char* encrypted = new unsigned char[packetSize];
		memcpy(encrypted, encryptedPacket->getBuffer(), packetSize);
		
		uint32_t rawPacketSize = packetSize - 0x08;
		unsigned char* rawPacket = new unsigned char[rawPacketSize];
		
		for (unsigned int i = 0; i < (rawPacketSize >> 0x03); i++) 
		{
			uint32_t xl = 0, xr = 0;
			ByteToLong(&encrypted[i * 8 + 0x07], &xl, &xr);
			blowfish_decrypt(&session.clientCtx, &xl, &xr);
			LongToByte(&rawPacket[i * 8], &xl, &xr);
		}

		memcpy(decryptedPacket->getBuffer(), rawPacket, rawPacketSize);

		return true;
	}

	ActiveMethod<bool, packetInfo, AlefLoginPacketHandler, ActiveStarter<ActiveDispatcher>> packetHandler;

private:
	bool packetProcessor(const packetInfo &packet)
	{
		if (packet.PacketType != 0x48)
		{
			AlefPacket * decryptedPacket = new AlefPacket();
			decryptPacket(packet.cryptoSession, packet.packet, decryptedPacket);
			cout << "Decrypted Opcode " << (int)decryptedPacket->GetPacketHeader(true).PacketType;
			HashMap<Int8, AlefPacketProcessor*>::Iterator Itr = processorMap.find(packet.PacketType);
			if (Itr != processorMap.end())
			{
				bool rtn = Itr->second->processPacket(packet.sock, decryptedPacket, packet.cryptoSession);
				delete decryptedPacket;
				return rtn;
			}
			else
			{
				delete decryptedPacket;
				return false;
			}
		}
		else //0x48 Handler TODO: Fix this mess
		{
			HashMap<Int8, AlefPacketProcessor*>::Iterator Itr = processorMap.find(packet.PacketType);
			if (Itr != processorMap.end())
			{
				return Itr->second->processPacket(packet.sock, packet.packet, packet.cryptoSession);
			}
			else
				return false;
		}
		
	}

	HashMap<Int8, AlefPacketProcessor*> processorMap;
};