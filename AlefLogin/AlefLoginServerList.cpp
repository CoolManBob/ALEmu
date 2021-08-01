#include "AlefLoginServerSystems.h"
#include "AlefLoginServerList.h"

bool AlefLoginServerList::processPacket(const localInfo& local)
{
	//AuPacket::GetField(&this->m_csPacket, 1, v8, v7, &pvPacket, &nSize, &Src, &nStatus, &pszEncodedWorld);
	localInfo& localObj = const_cast<localInfo&>(local);
	AlefPacket* packet = localObj.packet;
	Int8 i8Operation = 0;
	Int16 i16EncodedSize = 0;
	UInt32 i32Size = 0;
	string cUnk1 = "", cUnk2 = "", encodedWorld = "";
	cUnk1.reserve(32);
	cUnk2.reserve(32);
	
	//{	Alef::INT8, Alef::CHAR, Alef::INT16, Alef::MEMORY_BLOCK, Alef::CHAR }
	//{	1, 32, 1, 1, 32 }
	pktInterface->processPacket(packet, &i8Operation, &cUnk1, &i16EncodedSize, &i32Size, &encodedWorld, &cUnk2);

	switch (i8Operation)
	{
		case SERVERLIST_REQ: //ServerList Request Packet Opcode
			return processServerList(localObj); break;
		case 3:
			return processServerExtra(localObj); break;
		default:
		{
			stringstream errorMsg;
			errorMsg << "Server List processPacket Unhandled Operation: " << (int)i8Operation;
			LOG(errorMsg.str(), FATAL);
			return false;
		}
	}
	return false;
}

bool AlefLoginServerList::processServerList(localInfo& local)
{
	Int8 i8Operation = SERVERLIST_ANS;

	string worldListStr = serverListSys->buildWorldInfoStr();
	if (worldListStr == "ERROR")
	{
		LOG("processServerList: BAD worlistListStr", FATAL);
		return false;
	}
	UInt32 worldListLen = (UInt32)worldListStr.length()+1;
	
	SharedPtr<AlefPacket> serverList = pktInterface->buildPacket(Alef::AGPMWORLD_PACKET_TYPE, &i8Operation, 0, 0, &worldListLen, &worldListStr, 0);

	_localSock.sendPacket(serverList);

	return true;
}

bool AlefLoginServerList::processServerExtra(localInfo& local)
{
	return true;
}