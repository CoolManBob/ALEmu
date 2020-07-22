#include "AlefLoginServerList.h"

bool AlefLoginServerList::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	Int8 i8Operation = 0;
	char unkChar[32] = { 0 }, unkChar2[32] = { 0 };
	Int16 i16EncodedSize;
	char * encodedWorld = nullptr;
	//Int16 i16Status = 0;
	//int Src = 0;
	//int nStatus;
	
	//{	Alef::INT8, Alef::CHAR, Alef::INT16, Alef::MEMORY_BLOCK, Alef::CHAR }
	//{	1, 32, 1, 1, 32 }
	pktInterface->processPacket(packet, &i8Operation, unkChar, &i16EncodedSize, encodedWorld, unkChar2, 0);

	switch (i8Operation)
	{
		case SERVERLIST_REQ: //ServerList Request Packet Opcode 0x18 0x15 0x00
			return processServerList(sock, packet); break;
		default:
			return true;
	}
	return false;
}

bool AlefLoginServerList::processServerList(AlefSocket& sock, AlefPacket* packet)
{

	Int8 i8Operation = SERVERLIST_ANS;
	//WorldString Group-ServerName-Priority-Status-IsNew-NoEffect-NoEffect-NoEffect-NoEffect-
	unsigned char serverListStr[] = "Dummy=ALEmu=0=4=1=0=0=0=0;";
	unsigned int serverListStrLen = strlen((char*)serverListStr)+1;

	SharedPtr<AlefPacket> serverList = pktInterface->buildPacket(Alef::AGPMWORLD_PACKET_TYPE, &i8Operation, 0, 0, &serverListStrLen, serverListStr, 0);

	sock.sendPacket(serverList);
	return true;
}