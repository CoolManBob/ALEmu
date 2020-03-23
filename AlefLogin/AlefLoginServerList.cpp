#include "AlefLoginServerList.h"

bool AlefLoginServerList::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	Int8 i8Operation = 0;
	char unkChar[32] = { 0 };
	Int16 i16EncodedSize;
	char * encodedWorld = nullptr;
	//Int16 i16Status = 0;
	//int Src = 0;
	//int nStatus;
	
	/*{ Alef::INT8, Alef::CHAR, Alef::INT16, Alef::MEMORY_BLOCK }*/
	/*1, 32, 1, 1*/
	pktInterface->processPacket(packet, &i8Operation, unkChar, &i16EncodedSize, encodedWorld);

	//switch (packet->GetPacketFlag(FlagIndex::FLAG_IDX0)) //Opcode2
	switch (i8Operation) //Opcode2
	{
		case 0: //ServerList Request Packet Opcode 0x18 0x15 0x00
			return processServerList(sock, packet); break;
		default:
			return true;
	}
	return false;
}

bool AlefLoginServerList::processServerList(AlefSocket& sock, AlefPacket* packet)
{

	Int8 i8Operation = 2;
	unsigned char serverListStr[] = "Dummy=ALEmu=0=1=0;";
	unsigned int serverListStrLen = strlen((char*)serverListStr)+1;

	SharedPtr<AlefPacket> serverList = pktInterface->buildPacket(Alef::AGPMWORLD_PACKET_TYPE, &i8Operation, 0, 0, &serverListStrLen, serverListStr);

	sock.sendPacket(serverList);


	return true;
}