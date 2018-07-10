#include "AlefLoginServerList.h"

bool AlefLoginServerList::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	switch (packet->GetPacketHeader(true).PacketFlag)
	{
		case 0x15: //ServerList Request Packet Opcode 0x18 0x15 0x00
			return processServerList(sock, packet); break;
		default:
			return false;
	}
	return false;
}

bool AlefLoginServerList::processServerList(AlefSocket& sock, AlefPacket* packet)
{
	AlefPacket serverList(0x25, 0x18, 0x09, 0x02);

	unsigned char serverListStr[] = "Dummy=ALEmu=0=1=0;";
	unsigned int serverListStrLen = strlen((char*)serverListStr);
	serverList.WriteUInt16(serverListStrLen);
	serverList.WriteByteArray(serverListStr);
	serverList.WriteUInt8(0);
	serverList.WriteFooter();

	sock.sendPacket(&serverList);

	return true;
}