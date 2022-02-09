#include "AlefWorldAGSMCharMgr.h"

bool AlefWorldAGSMCharMgr::processPacket(const localInfo& local)
{
	/*AuPacket::GetField(
    &this->m_csPacket,
    1,
    pvPacket,
    nSize,
    &pvFirst,
    &v51,
    &stPosition,
    &lCID,
    &pcsAgsdCharacter,
    &v54,
    &v45);*/
	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	//{	1, 12, 1, 49, 1, 1, 1 }
	localInfo& localObj = const_cast<localInfo&>(local);
	AlefPacket* packet = localObj.packet;
	Int8 i8Operation = 0;
	string cUnk1 = "", cCharName = "";
	Int32 i32Unk1 = 0, i32Unk2 = 0, authToken = 0;
	Alef::AlefVec3F vPos;
	cUnk1.reserve(12);
	cCharName.reserve(49);
	pktInterface->processPacket(packet, &i8Operation, &cUnk1, &i32Unk2, &cCharName, &i32Unk2, &vPos, &authToken);
	switch (i8Operation)
	{
		case 2:
			return processGameEnterCharacterName(localObj, cCharName, authToken); break;
		case 9:
			return processEnterWorld(localObj); break;
		default:
		{
			stringstream errorMsg;
			errorMsg << "AGSMCharMgr processPacket Unhandled Operation: " << (int)i8Operation;
			LOG(errorMsg.str(), FATAL);
			return false;
		}
	}
	return false;
}

bool AlefWorldAGSMCharMgr::processGameEnterCharacterName(localInfo& local, string charName, Int32 authToken)
{
	LOG("processGameEnterCharacterName");

	//check the authKey and get the acctID
	//use acctID and charName to get the characterData
	UInt32 acctID = serverCharSys->getAcctID(authToken);
	_localSys->setAcctID(acctID);

	CharacterData curChar = serverCharSys->getCharacter(acctID, charName);
	_localSys->_localChar()->setCurChar(curChar);

	//{	Alef::INT32, Alef::INT32, Alef::INT8 }
	Int8 i8IsTestServer = 0;
	Int32 i32PCDropItemOnDeath = 0, i32ExpPenaltyOnDeath = 0;
	SharedPtr<AlefPacket> configPacket = pktInterface->buildPacket(Alef::AGPMCONFIG_PACKET_TYPE, &i32PCDropItemOnDeath, &i32ExpPenaltyOnDeath, &i8IsTestServer);

	_localSock.sendPacket(configPacket);

	//Refer to sendDummyCharacter for field information
	Int8 i8Operation = 25;
	Int32 i32CID = _curChar->getCharID();
	UInt32 u32eventEffectID = 0;
	SharedPtr<AlefPacket> eventEffectPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32CID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &u32eventEffectID, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(eventEffectPkt);

	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	Alef::AlefVec3F loadingPos = _curChar->getCurPos();
	i8Operation = 8;
	SharedPtr<AlefPacket> loadingPosPkt = pktInterface->buildPacket(Alef::AGSMCHARMANAGER_PACKET_TYPE, &i8Operation, 0, 0, 0, &i32CID, &loadingPos, 0);

	_localSock.sendPacket(loadingPosPkt);

	//{	Alef::UINT8, Alef::UINT64 }
	i8Operation = 6;
	UInt64 u64Timestamp = 0;
	SharedPtr<AlefPacket> timerPkt = pktInterface->buildPacket(Alef::AGPMTIMER_PACKET_TYPE, &i8Operation, &u64Timestamp);

	_localSock.sendPacket(timerPkt);

	return true;
}

bool AlefWorldAGSMCharMgr::processEnterWorld(localInfo& local)
{
	LOG("processEnterWorld");

	SharedPtr<AlefPacket> charPkt = _curChar->getCharPkt();
	_localSock.sendPacket(charPkt);

	//Send Title List
	/*struct PACKET_AGPPTITLE
	{
			PACKETHEADER header;
			byte FlagLow; //FlagLength = 2
			word nParam; //
			char strCharName[49];
			dword nTitleID;
	}; //PACKET_AGPPTITLE*/

	//Send TitleQuest List

	//Send CharView Packets
	/*{	Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::INT8, Alef::PACKET, Alef::INT8, Alef::INT8, Alef::VEC3F,
		Alef::VEC3F, Alef::INT32, Alef::UINT16, Alef::INT16, Alef::INT16, Alef::UINT64, Alef::INT8, Alef::INT8, Alef::INT8,
		Alef::INT8, Alef::UINT16, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32 }*/
	//SharedPtr<AlefPacket> charViewPkt = pktInterface->buildMiniPacket(Alef::AGPMOPTIMIZEDPACKET_VIEW, 0);

	
	/*{	Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::MEMORY_BLOCK, Alef::MEMORY_BLOCK, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::MEMORY_BLOCK,
		Alef::MEMORY_BLOCK, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK }*/
	Int8 i8Operation = 2;
	Int16 i16emptyBlock = 0;
	SharedPtr<AlefPacket> updateViewPkt = pktInterface->buildPacket(Alef::AGPMOPTIMIZEDVIEW_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, &i16emptyBlock, &i16emptyBlock, &i16emptyBlock, 0, 0, 0, 0, 0);

	_localSock.sendPacket(updateViewPkt);

	updateViewPkt = pktInterface->buildPacket(Alef::AGPMOPTIMIZEDVIEW_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, &i16emptyBlock, &i16emptyBlock, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(updateViewPkt);

	//{	Alef::INT32, Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::UINT16, Alef::INT8, Alef::INT8, Alef::INT8, Alef::INT32 }
	Int32 i32ID = _curChar->getCharID();
	Alef::AlefVec3F loadingPos = _curChar->getCurPos();
	Int8 i8Unk1 = 12, i8Zero = 0;
	SharedPtr<AlefPacket> updateMovePkt = pktInterface->buildPacket(Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE, &i32ID, &loadingPos, &loadingPos, 0, 0, &i8Unk1, &i8Zero, 0, 0);

	_localSock.sendPacket(updateMovePkt);

	//Add character to map

	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	i8Operation = 7;
	SharedPtr<AlefPacket> finishEnterPkt = pktInterface->buildPacket(Alef::AGSMCHARMANAGER_PACKET_TYPE, &i8Operation, 0, 0, 0, &i32ID, 0, 0);

	_localSock.sendPacket(finishEnterPkt);

	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET,
		Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
		Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
		Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 }*/

	SharedPtr<AlefPacket> charMovePkt = _curChar->getCharMovePkt();

	i8Operation = 1;
	Int8 i8Status = 2;
	SharedPtr<AlefPacket> charUpdatePkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32ID, 0, 0, &i8Status, 0, charMovePkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charUpdatePkt);

	SharedPtr<AlefPacket> factorPkt = _curChar->getCharFactorPkt();

	i8Operation = 1;
	SharedPtr<AlefPacket> charFactorUpdPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32ID, 0, 0, &i8Status, 0, 0, 0, factorPkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charFactorUpdPkt);

	//Send PassiveSkill

	return true;
}