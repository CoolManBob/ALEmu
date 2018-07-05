#include "AlefLoginClientLogin.h"

bool AlefLoginClientLogin::processPacket(AlefSocket& sock, AlefPacket * packet)
{
	switch (packet->GetPacketHeader(true).PacketFlag)
	{
	case 1: //Initial Packet Opcode 0x0D 0x01
		return processInitialLoginPacket(sock, packet); break;
	default:
		return false;
	}
	return false;
}

bool AlefLoginClientLogin::processInitialLoginPacket(AlefSocket& sock, AlefPacket * packet)
{
	AlefPacket response(0x1A, 0x0D, 0x03, 0x00);

	/*for (int i = 0; i <= 8; i++)
		response.WriteUInt8(0x00 + i);*/

	response.WriteUInt8(0);
	response.WriteUInt8(1);
	response.WriteUInt8(2);
	response.WriteUInt8(3);
	response.WriteUInt8(4);
	response.WriteUInt8(5);
	response.WriteUInt8(6);
	response.WriteUInt8(7);
	response.WriteUInt8(8);
	response.WriteUInt8(0x00);
	response.WriteUInt8(0x6B);

	sock.sendPacket(&response);
	return true;
}