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

	//{	Alef::INT32, Alef::INT32, Alef::INT8 }
	Int8 i8IsTestServer = 0;
	Int32 i32PCDropItemOnDeath = 0, i32ExpPenaltyOnDeath = 0;
	SharedPtr<AlefPacket> configPacket = pktInterface->buildPacket(Alef::AGPMCONFIG_PACKET_TYPE, &i32PCDropItemOnDeath, &i32ExpPenaltyOnDeath, &i8IsTestServer);

	_localSock.sendPacket(configPacket);

	//Refer to sendDummyCharacter for field information
	Int8 i8Operation = 25;
	Int32 i32CID = 1012;
	UInt32 u32eventEffectID = 0;
	SharedPtr<AlefPacket> eventEffectPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32CID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &u32eventEffectID, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(eventEffectPkt);

	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	Alef::AlefVec3F loadingPos(-466672, 3190.88, -46247.6);
	i8Operation = 8;
	Int32 i32ID = 1012;
	SharedPtr<AlefPacket> loadingPosPkt = pktInterface->buildPacket(Alef::AGSMCHARMANAGER_PACKET_TYPE, &i8Operation, 0, 0, 0, &i32ID, &loadingPos, 0);

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

	sendDummyCharacter(_localSock);

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
	Int32 i32ID = 1012;
	//Alef::AlefVec3F emptyVec;
	Alef::AlefVec3F loadingPos(-466672, 3190.88, -46247.6);
	Int8 i8Unk1 = 12, i8Zero = 0;
	SharedPtr<AlefPacket> updateMovePkt = pktInterface->buildPacket(Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE, &i32ID, &loadingPos, &loadingPos, 0, 0, &i8Unk1, &i8Zero, 0, 0);

	_localSock.sendPacket(updateMovePkt);

	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	i8Operation = 7;
	//i32ID = 1012;
	SharedPtr<AlefPacket> finishEnterPkt = pktInterface->buildPacket(Alef::AGSMCHARMANAGER_PACKET_TYPE, &i8Operation, 0, 0, 0, &i32ID, 0, 0);

	_localSock.sendPacket(finishEnterPkt);

	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET,
		Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
		Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
		Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 }*/
	//Alef::AlefVec3F loadingPos(-466672, 3190.88, -46247.6);
	Int8 i8MoveFlag = 4;
	Int32 i32Zero = 0;
	float fZero = 0;
	SharedPtr<AlefPacket> charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &loadingPos, &loadingPos, &i32Zero, &i32Zero, &fZero, &fZero, &i8MoveFlag, &i8Zero);

	i8Operation = 1;
	Int8 i8Status = 2;
	SharedPtr<AlefPacket> charUpdatePkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32ID, 0, 0, &i8Status, 0, charMovePkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charUpdatePkt);

	//Send Factors
	Int32 i32Dummy = 100, i32One = 1;

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	Int32 i32MoveSpeed = 10000;
	SharedPtr<AlefPacket> factorStatusPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_STATUS, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, &i32Dummy, &i32Dummy, &i32MoveSpeed, &i32MoveSpeed, 0, &i32Zero, 0);

	//{	Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorTypePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_TYPE, &i32One, &i32One, &i32One);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32}
	SharedPtr<AlefPacket> factorPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINT, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorPointMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINTMAX, &i32Dummy, &i32Dummy, &i32Zero, &i32Zero, &i32Zero, 0, 0, 0, 0, 0, 0);

	//Alef::AGPMCHAR_FACTOR_RECOVERY Skipped

	//Alef::AGPMCHAR_FACTOR_ATTRIBUTE Skipped

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorAttributeEmpty = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDmgPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DMG, factorAttributeEmpty, factorAttributeEmpty);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDefPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DEFENSE, factorAttributeEmpty, factorAttributeEmpty);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	Int32 i32AtkRng = 50, i32AtkSpd = 60;
	SharedPtr<AlefPacket> factorAttackPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTACK, &i32Dummy, &i32AtkRng, &i32AtkSpd, 0, 0, 0, 0, 0);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorDirtPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DIRT, 0, 0, 0, 0, 0, &i32Zero, 0, &i32Zero);

	//Alef::AGPMCHAR_FACTOR_PRICE Skipped

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorResPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, 0, factorStatusPkt, factorTypePkt, 0, factorPointMaxPkt, 0, 0, factorDmgPkt, factorDefPkt, factorAttackPkt, 0, factorDirtPkt, 0);

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, factorResPkt, factorStatusPkt, factorTypePkt, factorPointPkt, factorPointMaxPkt, 0, 0, factorDmgPkt, factorDefPkt, factorAttackPkt, 0, factorDirtPkt, 0);

	i8Operation = 1;
	SharedPtr<AlefPacket> charFactorUpdPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &i32ID, 0, 0, &i8Status, 0, 0, 0, factorPkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charFactorUpdPkt);

	//Send PassiveSkill

	/*AlefPacket res(0x12, 0x07, 0x00);
	res.WriteUInt8(0);
	res.WriteUInt32(1012); //ID

	res.WriteUInt16(0x10); //Size
	res.WriteUInt8(0x0F);
	res.WriteUInt8(1);
	res.WriteUInt32(1012);
	res.WriteByteArray("Dummy#test");
	res.ClosePacket();

	_localSock.sendPacket(&res);*/

	return true;
}

void AlefWorldAGSMCharMgr::sendDummyCharacter(AlefSocket& sock)
{
	Int32 i32Dummy = 100, i32One = 1, i32Zero = 0;
	float fZero = 0;
	Int8 i8Zero = 0;


	//{	Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::INT32, Alef::FLOAT, Alef::FLOAT, Alef::INT8, Alef::INT8 }
	//Alef::AlefVec3F pos, destPos;
	Alef::AlefVec3F loadingPos(-466672, 3190.88, -46247.6);
	Int8 i8MoveFlag = 4;
	SharedPtr<AlefPacket> charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &loadingPos, &loadingPos, &i32Zero, &i32Zero, &fZero, &fZero, &i8MoveFlag, &i8Zero);

#pragma region FACTORPACKETS

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	Int32 i32MoveSpeed = 10000;
	SharedPtr<AlefPacket> factorStatusPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_STATUS, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, &i32Dummy, &i32Dummy, &i32MoveSpeed, &i32MoveSpeed, 0, &i32Zero, 0);

	//{	Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorTypePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_TYPE, &i32One, &i32One, &i32One);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32}
	SharedPtr<AlefPacket> factorPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINT, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorPointMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINTMAX, &i32Dummy, &i32Dummy, &i32Zero, &i32Zero, &i32Zero, 0, 0, 0, 0, 0, 0);

	//Alef::AGPMCHAR_FACTOR_RECOVERY Skipped

	//Alef::AGPMCHAR_FACTOR_ATTRIBUTE Skipped

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorAttributeEmpty = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDmgPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DMG, factorAttributeEmpty, factorAttributeEmpty);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDefPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DEFENSE, factorAttributeEmpty, factorAttributeEmpty);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	Int32 i32AtkRng = 50, i32AtkSpd = 60;
	SharedPtr<AlefPacket> factorAttackPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTACK, &i32Dummy, &i32AtkRng, &i32AtkSpd, 0, 0, 0, 0, 0);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorDirtPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DIRT, 0, 0, 0, 0, 0, &i32Zero, 0, &i32Zero);

	//Alef::AGPMCHAR_FACTOR_PRICE Skipped

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorResPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, 0, factorStatusPkt, factorTypePkt, 0, factorPointMaxPkt, 0, 0, factorDmgPkt, factorDefPkt, factorAttackPkt, 0, factorDirtPkt, 0);

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, factorResPkt, factorStatusPkt, factorTypePkt, factorPointPkt, factorPointMaxPkt, 0, 0, factorDmgPkt, factorDefPkt, factorAttackPkt, 0, factorDirtPkt, 0);

#pragma endregion	

	Int8 i8Operation = 0;
	Int16 i16Zero = 0;
	Int32 charID = 1012, charTID = 96, nameLen = 4;
	Int64 i64Zero = 0;
	unsigned char charName[] = "test", skillInit[33] = { 0 }, signature[] = "Signature";
	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET,
	Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
	Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
	Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 }*/
	SharedPtr<AlefPacket> charInfoPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &charID, &charTID, &nameLen, charName, &i8Zero, charMovePkt, 0, factorPkt, &i64Zero, &i64Zero, &i64Zero, &i8Zero,
		&i8Zero, &i32Zero, &i8Zero, &i8Zero, &i8Zero, &i64Zero, &i8Zero, skillInit, &i8Zero, &i8Zero, &i32Zero, &i8Zero, &i16Zero, &i32Zero,
		&i32Zero, signature, &i16Zero, 0, &i32Zero);

	sock.sendPacket(charInfoPkt);
}