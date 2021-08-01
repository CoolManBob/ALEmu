#include <iostream>
#include <string>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

#include "AlefLoginGlobal.h"
#include "AlefLoginServerSystems.h"
#include "AlefLoginClientLogin.h"
#include "AlefHash.h"

AlefLoginClientLogin::AlefLoginClientLogin()
{
	baseSessionID = 1;
	curWorld = "";
}

AlefLoginClientLogin::~AlefLoginClientLogin()
{

}

bool AlefLoginClientLogin::processPacket(const localInfo& local)
{
	/*AuPacket::GetField(
		v10,
		1,
		pvPacket,
		nSize,
		&pvFirst,
		&lResult,
		&pvCharDetailInfo,
		&lCID,
		&cAccountLen,
		&pszPassword,
		&lIsProtected,
		&pszAccountID,
		&pszExtraForForeign,
		&pszEncryptCode,
		&v35,
		&cPasswordLen,
		&pvPacketVersionInfo,
		&pvDetailServerInfo,
		&pszChallenge,
		&lIsLimited);*/
	/*  GetField(_param_3,(int)&this->m_pAgpmLogin->m_csPacket,&DAT_00000001,(short)param_2,_param_3,
           (int)&pszAccountID + 3,&uStack8,&pszExtraForForeign,&lIsProtected,&lCID,&cAccountLen,
           &lIsLimited,&pvCharDetailInfo,&pvPacketVersionInfo,&lResult,auStack4,&pszPassword,
           &cPasswordLen,&pszEncryptCode,&pvDetailServerInfo,&pszChallenge);*/
	/*Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32*/
	/* 1, 32, 49, 1, 33, 1, 1, 32, 1, 1, 1, 1, 2049, 5, 1, 1*/
	localInfo& localObj = const_cast<localInfo&>(local);
	AlefPacket* packet = localObj.packet;
	Int8 i8Operation = 0;
	string cKey = "", cAcct = "", cPW = "", cWorld = "", cUnk2 = "", cUnk3 = "";
	cKey.reserve(32);
	cAcct.reserve(49);
	cPW.reserve(33);
	cWorld.reserve(32);
	cUnk2.reserve(2049);
	cUnk3.reserve(5);
	UInt8 ui8AcctLen = 0, ui8PWLen = 0;
	Int32 i32Unk1 = 0, i32Unk2 = 0, i32IsLimited = 0, i32IsProtected = 0;
	SharedPtr<AlefPacket> pktCharInfo = new AlefPacket(Alef::AGPMLOGIN_CHAR_INFO), pktVersionInfo = new AlefPacket(Alef::AGPMLOGIN_VERSION_INFO), pktServerInfo = new AlefPacket(Alef::AGPMLOGIN_SERVER_INFO);

	pktInterface->processPacket(packet, &i8Operation, &cKey, &cAcct, &ui8AcctLen, &cPW, &ui8PWLen, &i32Unk1, &cWorld, &pktCharInfo, &pktVersionInfo, &i32Unk2, &pktServerInfo, &cUnk2, &cUnk3, &i32IsLimited, &i32IsProtected);
	switch(i8Operation)
	{
		case CLIENTLOGIN_HASHKEY: //Initial Packet Opcode
			return processHashKeyPacket(localObj); break;
		case CLIENTLOGIN_USERLOGIN: //User Login Packet Opcode
			return processUserLoginPacket(localObj, cAcct, ui8AcctLen, cPW, ui8PWLen); break;
		case CLIENTLOGIN_UNIONINFO: //User World Union Info
			return processUnionInfo(localObj, cWorld); break;
		case CLIENTLOGIN_CHARLIST: //User Character List Request
			return processCharacterList(localObj); break;
		case CLIENTLOGIN_WORLDENTER: //World Enter
			return processWorldEnterRequest(localObj, pktCharInfo); break;
		case CLIENTLOGIN_CHARCREATIONENTER: //Character Creation Request
			return processCharacterCreation(localObj); break;
		case CLIENTLOGIN_CREATECHAR: //Complete Character Creation
			return processCreateCharacter(localObj, pktCharInfo);
		case CLIENTLOGIN_DELETECHAR: //Delete Char
			return processDeleteCharacter(localObj, pktCharInfo);
		default:
		{
			stringstream errorMsg;
			errorMsg << "User Login processPacket Unhandled Operation: " << (int)i8Operation;
			LOG(errorMsg.str(), FATAL);
			return false;
		}
	}
	return false;
}

bool AlefLoginClientLogin::processHashKeyPacket(localInfo& local)
{
	LOG("processHashKeyPacket");

	//Can check for version information here

	Int8 i8Operation = CLIENTLOGIN_HASHKEY;

	string hashKey = _localSys->_localAcct()->generateHashKey();

	_localSys->_localAcct()->setLoginStep(HASHKEY);

	SharedPtr<AlefPacket> response = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, &hashKey, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(response);

	return true;
}

bool AlefLoginClientLogin::processUserLoginPacket(localInfo& local, string acct, UInt8 acctLen, string pw, UInt8 pwLen)
{
	LOG("processUserLoginPacket");
	string user = acct, pass = pw;
	UInt32 acctID = 0;

	if (!_localSys->_localAcct()->decryptUserInfo(user, acctLen, pass, pwLen))
	{
		LOG("processUserLoginPacket: decryptUserInfo FAIL!", FATAL);
		return false;
	}

	string hashPW = AlefHash::getHash(pass);

	if (!serverLoginSys->checkLogin(user, hashPW, acctID))
	{
		sendLoginResult(_localSock, INCORRECTUSER); //Incorrect User ID
		return true;
	}

	_localSys->_localAcct()->setLoginStep(AUTHENTICATED);
	
	if (acctID != 0)
		_localSys->setAcctID(acctID);
	else
	{
		LOG("ACCTID IS ZERO", FATAL);
		return false;
	}

	//get character data
	clientCharDataVec charData;
	serverLoginSys->getCharData(acctID, charData);
	_localSys->_localChar()->setCharData(charData);

	Int8 i8Operation = CLIENTLOGIN_USERLOGIN;
	Int32 i32IsLimited = 0, i32IsProtected = 0;
	SharedPtr<AlefPacket> signOnResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, acct, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &i32IsLimited, &i32IsProtected);

	_localSock.sendPacket(signOnResponse);

	return true;
}

bool AlefLoginClientLogin::processUnionInfo(localInfo& local, string worldName)
{
	curWorld = worldName; 
	int unionType = 0;
	//Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR
	SharedPtr<AlefPacket> miniCharInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, 0, 0, 0, &unionType, 0, 0, 0, 0);

	/*Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32*/
	Int8 i8Operation = CLIENTLOGIN_UNIONINFO;
	SharedPtr<AlefPacket> unionResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, &miniCharInfo, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(unionResponse);

	clientCharDataVec localCharacters = _localSys->_localChar()->getCharacters();
	Int32 charCount = _localSys->_localChar()->getCharCount();

	for (clientCharDataVec::iterator charItr = localCharacters.begin(); charItr != localCharacters.end(); charItr++)
	{
		CharacterData charData = *charItr->get();
		string characterName = charData.charName;
		Int32 slotIdx = charData.slot;

		SharedPtr<AlefPacket> charName = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, &characterName, &charCount, &slotIdx, 0, 0, 0, 0, 0);

		i8Operation = CLIENTLOGIN_CHARNAME;

		SharedPtr<AlefPacket> charNameResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, &charName, 0, 0, 0, 0, 0, 0, 0);

		_localSock.sendPacket(charNameResponse);
	}


	i8Operation = CLIENTLOGIN_CHARNAMEFINISH;
	string acct = _localSys->_localAcct()->getUserAcct();

	SharedPtr<AlefPacket> charNameFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, &acct, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charNameFinish);

	return true;
}

bool AlefLoginClientLogin::processCharacterList(localInfo& local)
{
	_localSys->_localAcct()->setLoginStep(CHARLIST);

	clientCharDataVec localCharacters = _localSys->_localChar()->getCharacters();

	for (clientCharDataVec::iterator charItr = localCharacters.begin(); charItr != localCharacters.end(); charItr++)
	{
		CharacterData charData = *charItr->get();
		SharedPtr<AlefPacket> charPacket = buildCharPacket(charData);
		_localSock.sendPacket(charPacket);
	}


	Int8 i8Operation = CLIENTLOGIN_CHARLISTFINISH;
	Int32 cID = 0;
	string acct = _localSys->_localAcct()->getUserAcct();

	SharedPtr<AlefPacket> sendCharInfoFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, &acct, 0, 0, 0, &cID, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(sendCharInfoFinish);

	return true;
}

bool AlefLoginClientLogin::processCharacterCreation(localInfo& local)
{
	_localSys->_localAcct()->setLoginStep(CHARCREATE);

	if (!sendBaseCharPackets(_localSock))
		return false;

	//{	Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR }
	Int32 numChar = 15;
	Int32 race = 1;
	SharedPtr<AlefPacket> loginCharInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, 0, &numChar, 0, 0, &race, 0, 0, 0);

	Int8 i8Operation = CLIENTLOGIN_CHARCREATIONENTER;
	SharedPtr<AlefPacket> sendCharCreateFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, &loginCharInfo, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(sendCharCreateFinish);

	return true;
}

bool AlefLoginClientLogin::processCreateCharacter(localInfo& local, SharedPtr<AlefPacket> charDetail)
{
	//{	Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR }
	Int32 tID = 0, faceIdx = 0, hairIdx = 0;
	Int32 unk1 = 0, unk2 = 0;
	string charName = "";
	charName.reserve(49);
	pktInterface->processPacket(charDetail.get(), &tID, &charName, 0, &unk1, &unk2, 0, &hairIdx, &faceIdx, 0);


	//Need to check for max chars.
	UInt32 charCount = _localSys->_localChar()->getCharCount();
	if (charCount == 3)
	{
		//Send error code
		return false;
	}

	//For some reason charName becomes malformed and POCO doesn't like it, so passing it into the function
	//results in the SQL query to have a NULL where the name should be, passing in the c_str seems to fix it.
	//Check if char name is already in use.
	if (!serverLoginSys->checkCharName(charName))
	{
		sendLoginResult(_localSock, 7);
		return false;
	}

	UInt32 acctID = _localSys->_localAcct()->getUserID();

	if (!serverLoginSys->createChar(acctID, tID, faceIdx, hairIdx, charName))
	{
		LOG("createChar Failed!", FATAL);
		return false;
	}

	UInt32 slotIdx = _localSys->_localChar()->getAvailableSlotNum();
	SharedPtr<AlefPacket> charCreateInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, &charName, &charCount, &slotIdx, 0, 0, 0, 0, 0);

	Int8 i8Operation = CLIENTLOGIN_CREATECHAR;
	SharedPtr<AlefPacket> sendCharCreateInfoFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, &charCreateInfo, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(sendCharCreateInfoFinish);

	//refresh character data - is there a better way to do this? It's not expensive, but kinda "unnecessary"
	clientCharDataVec charData;
	serverLoginSys->getCharData(acctID, charData);
	_localSys->_localChar()->setCharData(charData);

	//send char data
	CharacterData newCharData = _localSys->_localChar()->getCharAtSlot(slotIdx);
	newCharData.charStatus = 1;
	newCharData.isNewChar = 1;
	SharedPtr<AlefPacket> charInfo = buildCharPacket(newCharData);
	_localSock.sendPacket(charInfo);

	i8Operation = CLIENTLOGIN_CREATECHARFINISH;
	UInt32 charID = newCharData.charID;
	SharedPtr<AlefPacket> sendNewCharFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, &charID, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(sendNewCharFinish);

	return true;
}

bool AlefLoginClientLogin::processWorldEnterRequest(localInfo& local, SharedPtr<AlefPacket> charDetail)
{
	LOG("World Enter Request");
	_localSys->_localAcct()->setLoginStep(ENTERWORLD);
	UInt32 acctID = _localSys->_localAcct()->getUserID();

	//{	Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR }
	string charName = "";
	Int32 unk1 = 0;
	charName.reserve(49);
	pktInterface->processPacket(charDetail.get(), &unk1, &charName, 0, 0, 0, 0, 0, 0, 0);

	//{	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 }
	Int8 i8Operation = 11;
	Int32 authKey = serverLoginSys->generateAuthKey(acctID);
	string name = charName + '#' + curWorld;
	SharedPtr<AlefPacket> authKeyPkt = pktInterface->buildPacket(Alef::AGSMCHARMANAGER_PACKET_TYPE, &i8Operation, 0, 0, &name, 0, 0, &authKey);

	_localSock.sendPacket(authKeyPkt);

	//{Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR}
	Int32 i32TID = 96; //does this matter?
	SharedPtr<AlefPacket> miniCharInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, &i32TID, &name, 0, 0, 0, 0, 0, 0, 0);

	//{	Alef::CHAR }
	string serverAddress = serverListSys->getWorldAddress(curWorld);

	if (serverAddress == "ERROR")
	{
		LOG("processWorldEnterRequest : serverAddress ERROR", FATAL);
		return false;
	}
	
	SharedPtr<AlefPacket> miniServerInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_SERVER_INFO, &serverAddress);

	//{	Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32}
	i8Operation = CLIENTLOGIN_WORLDENTER;
	Int32 i32CID = 0; //don't think this matters either
	SharedPtr<AlefPacket> serverInfoPkt = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, &i32CID, 0, miniCharInfo, miniServerInfo, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(serverInfoPkt);

	return true;
}

bool AlefLoginClientLogin::processDeleteCharacter(localInfo& local, SharedPtr<AlefPacket> charDetail)
{
	//{	Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR }
	string charName = "";
	charName.reserve(49);
	pktInterface->processPacket(charDetail.get(), 0, &charName, 0, 0, 0, 0, 0, 0, 0);

	UInt32 acctID = _localSys->_localAcct()->getUserID();

	if (!serverLoginSys->deleteChar(acctID, charName))
		return false;

	//refresh character data
	clientCharDataVec charData;
	serverLoginSys->getCharData(acctID, charData);
	_localSys->_localChar()->setCharData(charData);

	SharedPtr<AlefPacket> charNamePkt = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, &charName, 0, 0, 0, 0, 0, 0, 0);

	Int8 i8Operation = CLIENTLOGIN_DELETECHAR;

	SharedPtr<AlefPacket> charNameResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, &charNamePkt, 0, 0, 0, 0, 0, 0, 0);

	_localSock.sendPacket(charNameResponse);

	return true;
}

void AlefLoginClientLogin::sendLoginResult(AlefSocket& sock, int loginStatus)
{
	SharedPtr<AlefPacket> charName = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	Int8 i8Operation = CLIENTLOGIN_LOGINRESULT;

	SharedPtr<AlefPacket> loginResult = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, charName, 0, &loginStatus, 0, 0, 0, 0, 0);

	sock.sendPacket(loginResult);
}

bool AlefLoginClientLogin::sendBaseCharPackets(AlefSocket& sock)
{
	SharedPtr<AlefPacket> baseChar = getBaseCharPacket(BASECHAR::KNIGHT);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::ARCHER);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::MAGE);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::ZERKER);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::HUNTER);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::SORC);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::SWASH);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::RANGER);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::ELEMENTALIST);

	sock.sendPacket(baseChar);

	baseChar = getBaseCharPacket(BASECHAR::SCION);

	sock.sendPacket(baseChar);

	return true;
}


SharedPtr<AlefPacket> AlefLoginClientLogin::getBaseCharPacket(BASECHAR base)
{
	SharedPtr<CharDataInfo> baseChar;

	baseChar = serverDataSys->getCharDataFromDBID(static_cast<UInt32>(base));

	if (!baseChar)
		return nullptr;

	CharacterData charData;

	charData.charID = baseSessionID++;
	charData.charTID = baseChar->tID;
	charData.charName = baseChar->name;
	charData.charStatus = 2;
	//MovePacket
	
	charData.charFactors.result.status.stamina = baseChar->CON * 100;
	charData.charFactors.result.status.strength = baseChar->STR * 100;
	charData.charFactors.result.status.intelligence = baseChar->INT * 100;
	charData.charFactors.result.status.dexterity = baseChar->DEX * 100;
	charData.charFactors.result.status.charisma = baseChar->CHA * 100;
	charData.charFactors.result.status.wisdom = baseChar->WIS * 100;
	charData.charFactors.result.status.level = baseChar->LVL;
	charData.charFactors.result.status.moveSpeed = baseChar->walkSpeed;
	charData.charFactors.result.status.runSpeed = baseChar->runSpeed;

	charData.charFactors.result.type.charRace = baseChar->race;
	charData.charFactors.result.type.charGender = baseChar->gender;
	charData.charFactors.result.type.charClass = baseChar->clss;

	charData.charFactors.result.point.hp = baseChar->MAXHP;
	charData.charFactors.result.point.mana = baseChar->MAXMP;
	charData.charFactors.result.point.skillPoints = baseChar->MAXSP;

	charData.charFactors.result.pointMax.maxHP = baseChar->MAXHP;
	charData.charFactors.result.pointMax.maxMana = baseChar->MAXMP;
	charData.charFactors.result.pointMax.maxSkillPoints = baseChar->MAXSP;
	charData.charFactors.result.pointMax.xpHigh = 48; //level1 xp; will be replaced with data from DB

	charData.charFactors.result.attack.range = baseChar->range;
	charData.charFactors.result.attack.hitRange = 50; //Hardcoded value, check where to find real value; located in template fields
	charData.charFactors.result.attack.attackSpeed = baseChar->atkSpeed;

	charData.charFactors.status.stamina = baseChar->CON * 100;
	charData.charFactors.status.strength = baseChar->STR * 100;
	charData.charFactors.status.intelligence = baseChar->INT * 100;
	charData.charFactors.status.dexterity = baseChar->DEX * 100;
	charData.charFactors.status.charisma = baseChar->CHA * 100;
	charData.charFactors.status.wisdom = baseChar->WIS * 100;
	charData.charFactors.status.level = baseChar->LVL;
	charData.charFactors.status.moveSpeed = baseChar->walkSpeed;
	charData.charFactors.status.runSpeed = baseChar->runSpeed;

	charData.charFactors.type.charRace = baseChar->race;
	charData.charFactors.type.charGender = baseChar->gender;
	charData.charFactors.type.charClass = baseChar->clss;

	charData.charFactors.point.hp = baseChar->MAXHP;
	charData.charFactors.point.mana = baseChar->MAXMP;
	charData.charFactors.point.skillPoints = baseChar->MAXSP;

	charData.charFactors.pointMax.maxHP = baseChar->MAXHP;
	charData.charFactors.pointMax.maxMana = baseChar->MAXMP;
	charData.charFactors.pointMax.maxSkillPoints = baseChar->MAXSP;
	charData.charFactors.pointMax.xpHigh = 48; //level1 xp

	charData.charFactors.attack.range = baseChar->range;
	charData.charFactors.attack.hitRange = 50; //Hardcoded value, check where to find real value
	charData.charFactors.attack.attackSpeed = baseChar->atkSpeed;


	return buildCharPacket(charData);
}

SharedPtr<AlefPacket> AlefLoginClientLogin::buildCharPacket(CharacterData& data)
{
	CharacterMove charMove = data.charMove;
	CharacterFactors charFactors = data.charFactors;

	SharedPtr<AlefPacket> charMovePkt = buildCharMovePacket(charMove);
	SharedPtr<AlefPacket> charActionPkt = nullptr;
	SharedPtr<AlefPacket> factorPkt = buildFactorPacket(charFactors);

	Int32 nameLen = (Int32)data.charName.length();
	Int8 i8Operation = 0;
	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET,
	Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
	Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
	Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 }*/
	SharedPtr<AlefPacket> charInfoPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &data.charID, &data.charTID, &nameLen, &data.charName, &data.charStatus, charMovePkt, /*charActionPkt*/0, factorPkt,
		&data.money, &data.bankMoney, &data.cashMoney, &data.actionStatus, &data.criminalStatus, &data.attackerID, &data.isNewChar, &data.lastBindingIdx, &data.socialAnimIdx, &data.specialStatus, &data.transformStatus, &data.skillInit, 
		&data.face, &data.hair, &data.optionFlag, &data.bankSize, &data.eventStatusFlag, &data.remainingCrimTime, &data.remainingMurderTime, &data.signature, &data.GGSize, 0, &data.lastKilledBSquare);

	return charInfoPkt;
}

SharedPtr<AlefPacket> AlefLoginClientLogin::buildCharMovePacket(CharacterMove& move)
{
	SharedPtr<AlefPacket> charMovePkt;

	//{	Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::INT32, Alef::FLOAT, Alef::FLOAT, Alef::INT8, Alef::INT8 }
	charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &move.curPos, &move.destPos, &move.followTargetId, &move.followDistance, &move.axisX, &move.axisY, &move.flags, &move.direction);

	return charMovePkt;
}

SharedPtr<AlefPacket> AlefLoginClientLogin::buildFactorPacket(CharacterFactors& factors)
{
	CharacterFactorStatus charStatus = factors.status;
	CharacterFactorType charType = factors.type;
	CharacterFactorPoint charPoint = factors.point;
	CharacterFactorPointMax charPointMax = factors.pointMax;
	CharacterFactorRecovery charRecovery = factors.recovery;
	CharacterFactorAttribute charAttribute = factors.attribute;
	CharacterFactorDamage charDmg = factors.damage;
	CharacterFactorDefense charDef = factors.defense;
	CharacterFactorAttack charAtk = factors.attack;
	CharacterFactorDirt charDirt = factors.dirt;
	CharacterFactorPrice charPrice = factors.price;
	CharacterFactorItem charItem = factors.item;
	CharacterFactorResult charRes = factors.result;

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorStatusPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_STATUS, 
		&charStatus.stamina, &charStatus.strength, &charStatus.intelligence, &charStatus.dexterity, &charStatus.charisma, &charStatus.luck, 
		&charStatus.wisdom, &charStatus.level, &charStatus.moveSpeed, &charStatus.runSpeed, &charStatus.unionRank, &charStatus.villainPoints, &charStatus.unk);

	//{	Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorTypePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_TYPE, &charType.charRace, &charType.charGender, &charType.charClass);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32}
	SharedPtr<AlefPacket> factorPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINT, &charPoint.hp, &charPoint.mana, &charPoint.skillPoints, &charPoint.xpLow, 
		&charPoint.xpHigh, &charPoint.AP, &charPoint.MAP, &charPoint.MI, &charPoint.agro, &charPoint.damage, &charPoint.damageMagic, &charPoint.damageWater, &charPoint.damageFire, 
		&charPoint.damageEarth, &charPoint.damageAir, &charPoint.damagePoison, &charPoint.damageLightning, &charPoint.damageIce, &charPoint.bonusXP, &charPoint.damageHeroic);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorPointMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINTMAX, &charPointMax.maxHP, &charPointMax.maxMana, &charPointMax.maxSkillPoints, 
		&charPointMax.xpLow, &charPointMax.xpHigh, &charPointMax.AP, &charPointMax.MAP, &charPointMax.MI, &charPointMax.AR, &charPointMax.DR, &charPointMax.MAR);

	//Alef::AGPMCHAR_FACTOR_RECOVERY Skipped
	SharedPtr<AlefPacket> factorRecoveryPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_RECOVERY, &charRecovery.hpRecover, &charRecovery.manaRecovery, &charRecovery.spRecovery);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorAttributePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charAttribute.physical, &charAttribute.magic, &charAttribute.water, &charAttribute.fire,
		&charAttribute.earth, &charAttribute.air, &charAttribute.poison, &charAttribute.lightning, &charAttribute.ice, &charAttribute.physBlock, &charAttribute.skillBlock, &charAttribute.heroic,
		&charAttribute.heroicMelee, &charAttribute.heroicRange, &charAttribute.heroicMagic);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDmgMinPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDmg.min.physical, &charDmg.min.magic, &charDmg.min.water, &charDmg.min.fire,
		&charDmg.min.earth, &charDmg.min.air, &charDmg.min.poison, &charDmg.min.lightning, &charDmg.min.ice, &charDmg.min.physBlock, &charDmg.min.skillBlock, &charDmg.min.heroic,
		&charDmg.min.heroicMelee, &charDmg.min.heroicRange, &charDmg.min.heroicMagic);

	SharedPtr<AlefPacket> factorDmgMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDmg.max.physical, &charDmg.max.magic, &charDmg.max.water, &charDmg.max.fire,
		&charDmg.max.earth, &charDmg.max.air, &charDmg.max.poison, &charDmg.max.lightning, &charDmg.max.ice, &charDmg.max.physBlock, &charDmg.max.skillBlock, &charDmg.max.heroic,
		&charDmg.max.heroicMelee, &charDmg.max.heroicRange, &charDmg.max.heroicMagic);

	SharedPtr<AlefPacket> factorDmgPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DMG, factorDmgMinPkt, factorDmgMaxPkt);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDefPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDef.point.physical, &charDef.point.magic, &charDef.point.water, &charDef.point.fire,
		&charDef.point.earth, &charDef.point.air, &charDef.point.poison, &charDef.point.lightning, &charDef.point.ice, &charDef.point.physBlock, &charDef.point.skillBlock, &charDef.point.heroic,
		&charDef.point.heroicMelee, &charDef.point.heroicRange, &charDef.point.heroicMagic);

	SharedPtr<AlefPacket> factorDefRatePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDef.rate.physical, &charDef.rate.magic, &charDef.rate.water, &charDef.rate.fire,
		&charDef.rate.earth, &charDef.rate.air, &charDef.rate.poison, &charDef.rate.lightning, &charDef.rate.ice, &charDef.rate.physBlock, &charDef.rate.skillBlock, &charDef.rate.heroic,
		&charDef.rate.heroicMelee, &charDef.rate.heroicRange, &charDef.rate.heroicMagic);

	SharedPtr<AlefPacket> factorDefPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DEFENSE, factorDefPointPkt, factorDefRatePkt);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	//Int32 i32AtkRng = 50, i32AtkSpd = 60;
	SharedPtr<AlefPacket> factorAttackPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTACK, &charAtk.range, &charAtk.hitRange, &charAtk.attackSpeed, &charAtk.cast, &charAtk.cooldown, 
		&charAtk.accuracy, &charAtk.meleeDodge, &charAtk.rangeDodge);

	//
	SharedPtr<AlefPacket> factorItemPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ITEM, &charItem.durability, &charItem.hand, &charItem.rank, &charItem.physRank, &charItem.maxDura, &charItem.gacha);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorDirtPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DIRT, &charDirt.duration, &charDirt.intensity, &charDirt.range, &charDirt.target, &charDirt.skillLevel, &charDirt.skillPoint, &charDirt.skillXP, &charDirt.heroicPoint);

	//Alef::AGPMCHAR_FACTOR_PRICE Skipped
	SharedPtr<AlefPacket> factorPricePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_PRICE, &charPrice.npcPrice, &charPrice.playerPrice, &charPrice.moneyHigh, &charPrice.moneyLow);

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorResPkt = buildFactorResultPacket(charRes);

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, factorResPkt, factorStatusPkt, factorTypePkt, factorPointPkt, factorPointMaxPkt, factorRecoveryPkt, factorAttributePkt, factorDmgPkt, factorDefPkt, factorAttackPkt, factorItemPkt, factorDirtPkt, factorPricePkt);

	return factorPkt;
}

SharedPtr<AlefPacket> AlefLoginClientLogin::buildFactorResultPacket(CharacterFactorResult& result)
{
	CharacterFactorStatus charStatus = result.status;
	CharacterFactorType charType = result.type;
	CharacterFactorPoint charPoint = result.point;
	CharacterFactorPointMax charPointMax = result.pointMax;
	CharacterFactorRecovery charRecovery = result.recovery;
	CharacterFactorAttribute charAttribute = result.attribute;
	CharacterFactorDamage charDmg = result.damage;
	CharacterFactorDefense charDef = result.defense;
	CharacterFactorAttack charAtk = result.attack;
	CharacterFactorDirt charDirt = result.dirt;
	CharacterFactorPrice charPrice = result.price;
	CharacterFactorItem charItem = result.item;

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorStatusPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_STATUS,
		&charStatus.stamina, &charStatus.strength, &charStatus.intelligence, &charStatus.dexterity, &charStatus.charisma, &charStatus.luck,
		&charStatus.wisdom, &charStatus.level, &charStatus.moveSpeed, &charStatus.runSpeed, &charStatus.unionRank, &charStatus.villainPoints, &charStatus.unk);

	//{	Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorTypePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_TYPE, &charType.charRace, &charType.charGender, &charType.charClass);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32}
	SharedPtr<AlefPacket> factorPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINT, &charPoint.hp, &charPoint.mana, &charPoint.skillPoints, &charPoint.xpLow,
		&charPoint.xpHigh, &charPoint.AP, &charPoint.MAP, &charPoint.MI, &charPoint.agro, &charPoint.damage, &charPoint.damageMagic, &charPoint.damageWater, &charPoint.damageFire,
		&charPoint.damageEarth, &charPoint.damageAir, &charPoint.damagePoison, &charPoint.damageLightning, &charPoint.damageIce, &charPoint.bonusXP, &charPoint.damageHeroic);

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorPointMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_POINTMAX, &charPointMax.maxHP, &charPointMax.maxMana, &charPointMax.maxSkillPoints,
		&charPointMax.xpLow, &charPointMax.xpHigh, &charPointMax.AP, &charPointMax.MAP, &charPointMax.MI, &charPointMax.AR, &charPointMax.DR, &charPointMax.MAR);

	//Alef::AGPMCHAR_FACTOR_RECOVERY Skipped
	SharedPtr<AlefPacket> factorRecoveryPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_RECOVERY, &charRecovery.hpRecover, &charRecovery.manaRecovery, &charRecovery.spRecovery);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorAttributePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charAttribute.physical, &charAttribute.magic, &charAttribute.water, &charAttribute.fire,
		&charAttribute.earth, &charAttribute.air, &charAttribute.poison, &charAttribute.lightning, &charAttribute.ice, &charAttribute.physBlock, &charAttribute.skillBlock, &charAttribute.heroic,
		&charAttribute.heroicMelee, &charAttribute.heroicRange, &charAttribute.heroicMagic);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDmgMinPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDmg.min.physical, &charDmg.min.magic, &charDmg.min.water, &charDmg.min.fire,
		&charDmg.min.earth, &charDmg.min.air, &charDmg.min.poison, &charDmg.min.lightning, &charDmg.min.ice, &charDmg.min.physBlock, &charDmg.min.skillBlock, &charDmg.min.heroic,
		&charDmg.min.heroicMelee, &charDmg.min.heroicRange, &charDmg.min.heroicMagic);

	SharedPtr<AlefPacket> factorDmgMaxPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDmg.max.physical, &charDmg.max.magic, &charDmg.max.water, &charDmg.max.fire,
		&charDmg.max.earth, &charDmg.max.air, &charDmg.max.poison, &charDmg.max.lightning, &charDmg.max.ice, &charDmg.max.physBlock, &charDmg.max.skillBlock, &charDmg.max.heroic,
		&charDmg.max.heroicMelee, &charDmg.max.heroicRange, &charDmg.max.heroicMagic);

	SharedPtr<AlefPacket> factorDmgPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DMG, factorDmgMinPkt, factorDmgMaxPkt);

	//{ Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorDefPointPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDef.point.physical, &charDef.point.magic, &charDef.point.water, &charDef.point.fire,
		&charDef.point.earth, &charDef.point.air, &charDef.point.poison, &charDef.point.lightning, &charDef.point.ice, &charDef.point.physBlock, &charDef.point.skillBlock, &charDef.point.heroic,
		&charDef.point.heroicMelee, &charDef.point.heroicRange, &charDef.point.heroicMagic);

	SharedPtr<AlefPacket> factorDefRatePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTRIBUTE, &charDef.rate.physical, &charDef.rate.magic, &charDef.rate.water, &charDef.rate.fire,
		&charDef.rate.earth, &charDef.rate.air, &charDef.rate.poison, &charDef.rate.lightning, &charDef.rate.ice, &charDef.rate.physBlock, &charDef.rate.skillBlock, &charDef.rate.heroic,
		&charDef.rate.heroicMelee, &charDef.rate.heroicRange, &charDef.rate.heroicMagic);

	SharedPtr<AlefPacket> factorDefPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DEFENSE, factorDefPointPkt, factorDefRatePkt);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	//Int32 i32AtkRng = 50, i32AtkSpd = 60;
	SharedPtr<AlefPacket> factorAttackPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ATTACK, &charAtk.range, &charAtk.hitRange, &charAtk.attackSpeed, &charAtk.cast, &charAtk.cooldown,
		&charAtk.accuracy, &charAtk.meleeDodge, &charAtk.rangeDodge);

	SharedPtr<AlefPacket> factorItemPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_ITEM, &charItem.durability, &charItem.hand, &charItem.rank, &charItem.physRank, &charItem.maxDura, &charItem.gacha);

	//{ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorDirtPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_DIRT, &charDirt.duration, &charDirt.intensity, &charDirt.range, &charDirt.target, &charDirt.skillLevel, &charDirt.skillPoint, &charDirt.skillXP, &charDirt.heroicPoint);

	//Alef::AGPMCHAR_FACTOR_PRICE Skipped
	SharedPtr<AlefPacket> factorPricePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_PRICE, &charPrice.npcPrice, &charPrice.playerPrice, &charPrice.moneyHigh, &charPrice.moneyLow);

	//{	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET }
	SharedPtr<AlefPacket> factorResPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTORS, 0, factorStatusPkt, factorTypePkt, factorPointPkt, factorPointMaxPkt, factorRecoveryPkt, factorAttributePkt, factorDmgPkt, factorDefPkt, factorAttackPkt, factorItemPkt, factorDirtPkt, factorPricePkt);

	return factorResPkt;
}