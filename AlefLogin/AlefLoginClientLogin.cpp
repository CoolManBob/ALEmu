#include <iostream>
#include <string>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

#include "AlefLoginGlobal.h"
#include "AlefLoginClientLogin.h"
#include "AlefMD5Crypto.h"

AlefLoginClientLogin::AlefLoginClientLogin()
{

}

AlefLoginClientLogin::~AlefLoginClientLogin()
{

}

bool AlefLoginClientLogin::processPacket(AlefSocket& sock, AlefPacket * packet)
{
	/*Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32*/
	/* 1, 32, 49, 1, 33, 1, 1, 32, 1, 1, 1, 1, 2049, 5, 1, 1*/
	Int8 i8Operation = 0;
	unsigned char acct[49] = { 0 };
	UInt8 acctLen = 0, pwLen = 0;
	unsigned char pw[33] = { 0 };
	Int8 i8Var = 0;
	char char1Var[33] = { 0 };
	//char 

	pktInterface->processPacket(packet, &i8Operation, 0, acct, &acctLen, pw, &pwLen, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	//UInt8 flag = packet->GetPacketFlag(FlagIndex::FLAG_IDX0); //Opcode2
	switch(i8Operation)
	{
		case 0: //Initial Packet Opcode
			return processInitialLoginPacket(sock, packet); break;
		case 1: //User Login Packet Opcode
			return processUserLoginPacket(sock, packet, acct, acctLen, pw, pwLen); break;
		case 3: //User World Union Info
			return processUnionInfo(sock, packet); break;
		case 6: //User Character List Request
			return processCharacterList(sock, packet); break;
		case 0x05: //Character Creation Request 0x0D 0x05
			return processCharacterCreation(sock, packet); break;
		case 0xC5: //World Enter 0x0D 0xC5
			return processWorldEnterRequest(sock, packet); break;
		default:
		{
			stringstream errorMsg;
			errorMsg << "User Connect processPacket Unhandled Operation: " << (int)i8Operation;
			LOG(errorMsg.str(), FATAL);
			return false;
		}
	}
	return false;
}

bool AlefLoginClientLogin::processInitialLoginPacket(AlefSocket& sock, AlefPacket * packet)
{
	LOG("processInitialLoginPacket");

	Int8 i8Operation = 0, i8Unk = 0;
	unsigned char hashKey[] = "12345678";

	AlefPacket * response = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, hashKey, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); //MD5 Crypto Packet

	sock.sendPacket(response);

	delete response;
	return true;
}

bool AlefLoginClientLogin::processUserLoginPacket(AlefSocket& sock, AlefPacket* packet, unsigned char* acct, UInt8 acctLen, unsigned char* pw, UInt8 pwLen)
{
	LOG("processUserLoginPacket");
	AlefMD5Crypto md5Crypt;

	md5Crypt.decryptString(acct, acctLen);
	md5Crypt.decryptString(pw, pwLen);

	stringstream acctMsg;
	acctMsg << "Account: " << acct << " " << "Password: " << pw;
	LOG(acctMsg.str());

	/*AlefPacket signOnResponse(0x0D, 0x05, 0xC0);
	signOnResponse.WriteUInt8(0x01); //operation
	signOnResponse.WriteByteArray("test");
	for (int i = 0; i < 53; i++)
		signOnResponse.WriteUInt8(0);
	signOnResponse.ClosePacket();*/

	Int8 i8Operation = 1;
	Int32 i32Unk = 0;

	AlefPacket* signOnResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, acct, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &i32Unk, &i32Unk);

	sock.sendPacket(signOnResponse);

	delete signOnResponse;
	//delete[] md5account;
	//delete[] md5password;

	return true;
}

bool AlefLoginClientLogin::processUnionInfo(AlefSocket& sock, AlefPacket* packet)
{
	int unionType = 0;
	AlefPacket* miniCharInfo = pktInterface->buildMiniPacket(Alef::AGPMLOGIN_CHAR_INFO, 0, 0, 0, 0, &unionType, 0, 0, 0, 0);

	/*Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32*/
	Int8 i8Operation = 3;
	AlefPacket* unionResponse = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, 0, 0, 0, 0, 0, 0, miniCharInfo, 0, 0, 0, 0, 0, 0, 0);

	sock.sendPacket(unionResponse);

	i8Operation = 5;
	unsigned char acct[] = "acct";

	AlefPacket* charNameFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, acct, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	sock.sendPacket(charNameFinish);

	delete miniCharInfo;
	delete unionResponse;
	delete charNameFinish;

	return true;

	/*UInt8 operation = packet->GetPacketFlag(FlagIndex::FLAG_IDX1);
	switch (operation)
	{
		case 0x03:
		{
			LOG("Initial User Connect");
			AlefPacket response(0x0D, 0x01, 0x01);

			response.WriteUInt8(0x03);
			response.WriteUInt8(0x06);
			response.WriteUInt8(0x00);
			response.WriteUInt8(0x10);
			response.WriteUInt8(0x00);
			response.WriteUInt8(0x01);
			response.WriteUInt8(0x00);
			response.WriteUInt8(0x00);
			response.WriteUInt8(0x00);
			response.ClosePacket();

			sock.sendPacket(&response);

			//Send Character List
			AlefPacket charListRes(0x0D, 0x01, 0x01);

			charListRes.WriteUInt8(0x04); //Char List Operation
			
			charListRes.WriteUInt16(59); //Size
			charListRes.WriteUInt16(14); //Flag
			charListRes.WriteByteArray("test");
			for (int i = 0; i < 44; i++)
				charListRes.WriteUInt8(0); //Spacing
			charListRes.WriteUInt8(4); //name Length
			charListRes.WriteUInt8(1); //Unk
			for (int i = 0; i < 7; i++)
				charListRes.WriteUInt8(0); //spacing
			
			charListRes.ClosePacket();
			
			sock.sendPacket(&charListRes);

			AlefPacket response2(0x0D, 0x05, 0x00);
			response2.WriteUInt8(0x05);
			response2.WriteByteArray("test");
			for (int i = 0; i < 45; i++)
				response2.WriteUInt8(0);
			response2.ClosePacket();

			sock.sendPacket(&response2);
		}break;
		case 0x06: //Character List
		{
			LOG("Character List");

			AlefPacket charResponse(0x04, 0x11, 0x00);
			charResponse.WriteUInt8(0x40);
			charResponse.WriteUInt8(0);
			charResponse.WriteUInt8(0x13);
			charResponse.WriteUInt8(0x65);
			for (int i = 0; i < 7; i++)
				charResponse.WriteUInt8(0);
			charResponse.ClosePacket();
			sock.sendPacket(&charResponse);

			sendDummyCharacter(sock);

			AlefPacket charListComplete(0x02, 0x13, 0x00);
			charListComplete.WriteUInt16(0);
			charListComplete.WriteUInt8(0x01);
			charListComplete.WriteUInt8(0x65);
			charListComplete.WriteUInt16(0);
			charListComplete.WriteUInt8(0);
			charListComplete.WriteUInt8(0x01);
			charListComplete.ClosePacket();
			sock.sendPacket(&charListComplete);

			AlefPacket response(0x0D, 0x45, 0x00);
			response.WriteUInt8(0x07);
			response.WriteByteArray("test");
			for (int i = 0; i < 49; i++)
				response.WriteUInt8(0);
			response.ClosePacket();

			sock.sendPacket(&response);
		}break;
		default:
		{
			stringstream errorMsg;
			errorMsg << "User Connect Unhandled Operation: " << (int)operation;
			LOG(errorMsg.str(), WARNING);
			return false;
		} break;
	}*/
}

bool AlefLoginClientLogin::processCharacterList(AlefSocket& sock, AlefPacket* packet)
{
	Int8 i8Operation = 7;
	Int32 cID = 0;
	unsigned char acct[] = "acct";

	AlefPacket* sendCharInfoFinish = pktInterface->buildPacket(Alef::AGPMLOGIN_PACKET_TYPE, &i8Operation, 0, acct, 0, 0, 0, &cID, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	sock.sendPacket(sendCharInfoFinish);

	delete sendCharInfoFinish;

	return true;
}

bool AlefLoginClientLogin::processCharacterCreation(AlefSocket& sock, AlefPacket* packet)
{
	/*UInt8 operation = packet->GetPacketFlag(FlagIndex::FLAG_IDX1);
	switch (operation)
	{
		case 0x01:
		{
			LOG("Character Creation REQ");
			
			//Send Character Info (02 BF 9F)
			AlefPacket charInfo(0x02, 0xBF, 0x9F);

		} break;
		default:
		{
			stringstream warningMsg;
			warningMsg << "Character Creation Default Case Op: " << (int)operation;
			LOG(warningMsg.str(), WARNING);
		} break;
	}*/
	
	return true;
}

bool AlefLoginClientLogin::processWorldEnterRequest(AlefSocket& sock, AlefPacket* packet)
{
	LOG("World Enter Request");

	AlefPacket res1(0x03, 0x49, 0x0B);
	res1.WriteByteArray("Dummy#test");
	for (int i = 0; i < 36; i++)
		res1.WriteUInt8(0);
	res1.WriteUInt8(0x01);
	res1.WriteUInt8(0xDA);
	res1.WriteUInt8(0xFB);
	res1.WriteUInt8(0xEC);
	res1.ClosePacket();

	sock.sendPacket(&res1);

	AlefPacket worldServerResponse(0x0D, 0x41, 0x03);
	worldServerResponse.WriteUInt8(8);
	worldServerResponse.WriteUInt32(0); //CID
	
	worldServerResponse.WriteUInt16(10); //Char Name Detail Length (Flag + Int + Char Name Length)
	worldServerResponse.WriteUInt16(3); //Char Name Detail Flag
	worldServerResponse.WriteUInt32(96); //Char TID
	worldServerResponse.WriteByteArray("test");

	worldServerResponse.WriteUInt16(strlen("127.0.0.1:11008")+1); //Server Detail Length (strlen of address + flag)
	worldServerResponse.WriteUInt8(1); //Server Detail Flag
	worldServerResponse.WriteByteArray("127.0.0.1:11008");
	worldServerResponse.ClosePacket();

	sock.sendPacket(&worldServerResponse);

	return true;
}

void AlefLoginClientLogin::sendDummyCharacter(AlefSocket& sock)
{
	LOG("Dummy Character Data");
	AlefPacket charInfo(0x02, 0xBF, 0x9F);
	
	charInfo.WriteUInt8(0xBE); //1st Byte of PacketFlag(Character) Operation
	charInfo.WriteUInt8(0x2F); //2nd byte of PacketFlag(Character) operation

	charInfo.WriteUInt8(0); //Operation

	charInfo.WriteUInt32(1012); //Character ID
	charInfo.WriteUInt32(96); //Character TID

	charInfo.WriteUInt16(4); //Name Length
	charInfo.WriteByteArray("test");

	//charInfo.WriteUInt16(0); //BattleSquare
	charInfo.WriteUInt8(0); //Status

	/*Begin Movement Data - 43 Bytes + 2 Size*/
	charInfo.WriteUInt16(0x2B); //Move Packet Size
	charInfo.WriteUInt8(0xFF); //Move Header Flag

	charInfo.WriteFloat(0); //PosX
	charInfo.WriteFloat(0); //PosY
	charInfo.WriteFloat(0); //PosZ

	charInfo.WriteFloat(0); //DestPosX
	charInfo.WriteFloat(0); //DestPosY
	charInfo.WriteFloat(0); //DestPosZ
	
	charInfo.WriteUInt32(0); //followID
	charInfo.WriteUInt32(0); //followDist

	charInfo.WriteFloat(0); //TurnX
	charInfo.WriteFloat(0); //TurnY

	charInfo.WriteUInt8(0); //movementOperation (Flag)
	charInfo.WriteUInt8(0); //bindingRegionIndex
	/*End Movement Data*/

#pragma region FACTORDATA
	/*Start Factor Data*/

	/*Result Factor*/
	charInfo.WriteUInt16(302); //Total Factor Size
	charInfo.WriteUInt16(2975); //Total Factor Flag

	charInfo.WriteUInt16(137); //Factor Result Size
	charInfo.WriteUInt16(2966); //Factor Result Flag

	/*Result Character Status*/
	charInfo.WriteUInt16(42); //Result Status Size
	charInfo.WriteUInt16(3039); //Result Status Flag
	
	charInfo.WriteUInt32(100); //CON
	charInfo.WriteUInt32(100); //STR
	charInfo.WriteUInt32(100); //INT
	charInfo.WriteUInt32(100); //DEX
	charInfo.WriteUInt32(100); //CHA
	//charInfo.WriteUInt32(0); //LUK
	charInfo.WriteUInt32(100); //WIS
	charInfo.WriteUInt32(1); //Level
	charInfo.WriteUInt32(100); //WalkSpeed
	charInfo.WriteUInt32(100); //RunSpeed
	charInfo.WriteUInt32(0); //UnionRank
	/*charInfo.WriteUInt32(0); //VillainPoints
	charInfo.WriteUInt32(0); //filler?*/
	/*End Character Status*/

	/*Result Character Type*/
	charInfo.WriteUInt16(13); //Result Type Size
	charInfo.WriteUInt8(7); //Result Type Flag
	
	charInfo.WriteUInt32(1); //Race
	charInfo.WriteUInt32(1); //Gender
	charInfo.WriteUInt32(1); //Class
	/*End Character Type*/

	/*Character Point*/
	/*charInfo.WriteUInt32(100); //HP
	charInfo.WriteUInt32(100); //MP
	charInfo.WriteUInt32(0); //SP
	charInfo.WriteUInt32(0); //Exp
	charInfo.WriteUInt32(0); //ExpHigh
	charInfo.WriteUInt32(0); //AttackPoint
	charInfo.WriteUInt32(0); //MagicAttackPoint
	charInfo.WriteUInt32(0); //Magic Intensity
	charInfo.WriteUInt32(0); //Aggro*/
	/*End Character Point*/

	/*Result Point Max*/
	charInfo.WriteUInt16(22); //Result Point Max Size
	charInfo.WriteUInt16(31); //Result Point Max Flag
	
	charInfo.WriteUInt32(100); //HP
	charInfo.WriteUInt32(100); //MP
	charInfo.WriteUInt32(0); //SP
	charInfo.WriteUInt32(0); //Exp
	charInfo.WriteUInt32(0); //ExpHigh
	//charInfo.WriteUInt32(0); //AttackPoint
	//charInfo.WriteUInt32(0); //MagicAttackPoint
	//charInfo.WriteUInt32(0); //MagicIntensity
	//charInfo.WriteUInt32(0); //AR
	//charInfo.WriteUInt32(0); //DR
	//charInfo.WriteUInt32(0); //BaseEXP
	/*End Character Point Max*/

	/*Character Recovery Rate*/
	/*charInfo.WriteUInt32(0); //HP
	charInfo.WriteUInt32(0); //MP
	charInfo.WriteUInt32(0); //SP*/
	/*End Character Recovery*/

	/*Character Attribute*/
	/*charInfo.WriteUInt32(0); //Physical
	charInfo.WriteUInt32(0); //Magic
	charInfo.WriteUInt32(0); //Water
	charInfo.WriteUInt32(0); //Fire
	charInfo.WriteUInt32(0); //Earth
	charInfo.WriteUInt32(0); //AIR
	charInfo.WriteUInt32(0); //Poison
	charInfo.WriteUInt32(0); //Lightening
	charInfo.WriteUInt32(0); //Ice
	charInfo.WriteUInt32(0); //PhysicalBlock
	charInfo.WriteUInt32(0); //Skill
	charInfo.WriteUInt32(0); //Heroic
	charInfo.WriteUInt32(0); //Heroic_Melee
	charInfo.WriteUInt32(0); //Heroic_Ranged
	charInfo.WriteUInt32(0); //Heroic_Ranged*/
	/*End Character Attribute*/

	/*Result Factor Damage*/
	charInfo.WriteUInt16(9); //Result Damage Size
	charInfo.WriteUInt8(3); //Result Damage Flag

	charInfo.WriteUInt16(2); //Result Damage Minimum Size
	charInfo.WriteUInt16(0); //Result Damage Minimum Flag

	charInfo.WriteUInt16(2); //Result Damage Maximum Size
	charInfo.WriteUInt16(0); //Result Damage Maximum Flag

	//Minimum
	/*charInfo.WriteUInt32(0); //Physical
	charInfo.WriteUInt32(0); //Magic
	charInfo.WriteUInt32(0); //Water
	charInfo.WriteUInt32(0); //Fire
	charInfo.WriteUInt32(0); //Earth
	charInfo.WriteUInt32(0); //AIR
	charInfo.WriteUInt32(0); //Poison
	charInfo.WriteUInt32(0); //Lightening
	charInfo.WriteUInt32(0); //Ice
	charInfo.WriteUInt32(0); //PhysicalBlock
	charInfo.WriteUInt32(0); //Skill
	charInfo.WriteUInt32(0); //Heroic
	charInfo.WriteUInt32(0); //Heroic_Melee
	charInfo.WriteUInt32(0); //Heroic_Ranged
	charInfo.WriteUInt32(0); //Heroic_Ranged
	//Maximum
	charInfo.WriteUInt32(0); //Physical
	charInfo.WriteUInt32(0); //Magic
	charInfo.WriteUInt32(0); //Water
	charInfo.WriteUInt32(0); //Fire
	charInfo.WriteUInt32(0); //Earth
	charInfo.WriteUInt32(0); //AIR
	charInfo.WriteUInt32(0); //Poison
	charInfo.WriteUInt32(0); //Lightening
	charInfo.WriteUInt32(0); //Ice
	charInfo.WriteUInt32(0); //PhysicalBlock
	charInfo.WriteUInt32(0); //Skill
	charInfo.WriteUInt32(0); //Heroic
	charInfo.WriteUInt32(0); //Heroic_Melee
	charInfo.WriteUInt32(0); //Heroic_Ranged
	charInfo.WriteUInt32(0); //Heroic_Ranged*/
	/*End Character Damage*/

	/*Result Character Defense*/

	charInfo.WriteUInt16(13); //Result Defense Size
	charInfo.WriteUInt8(3); //Result Defense Flag

	//Defense Point - Minimum
	charInfo.WriteUInt16(6); //Result Defense Minimum Size
	charInfo.WriteUInt16(1); //Result Defense Minimum Flag

	charInfo.WriteUInt32(0); //Physical

	//Defense Point - Maximum
	charInfo.WriteUInt16(2); //Result Defense Maximum Size
	charInfo.WriteUInt16(0); //Result Defense Maximum Flag

	/*charInfo.WriteUInt32(0); //Physical
	charInfo.WriteUInt32(0); //Magic
	charInfo.WriteUInt32(0); //Water
	charInfo.WriteUInt32(0); //Fire
	charInfo.WriteUInt32(0); //Earth
	charInfo.WriteUInt32(0); //AIR
	charInfo.WriteUInt32(0); //Poison
	charInfo.WriteUInt32(0); //Lightening
	charInfo.WriteUInt32(0); //Ice
	charInfo.WriteUInt32(0); //PhysicalBlock
	charInfo.WriteUInt32(0); //Skill
	charInfo.WriteUInt32(0); //Heroic
	charInfo.WriteUInt32(0); //Heroic_Melee
	charInfo.WriteUInt32(0); //Heroic_Ranged
	charInfo.WriteUInt32(0); //Heroic_Ranged
	//DefenseRate
	charInfo.WriteUInt32(0); //Physical
	charInfo.WriteUInt32(0); //Magic
	charInfo.WriteUInt32(0); //Water
	charInfo.WriteUInt32(0); //Fire
	charInfo.WriteUInt32(0); //Earth
	charInfo.WriteUInt32(0); //AIR
	charInfo.WriteUInt32(0); //Poison
	charInfo.WriteUInt32(0); //Lightening
	charInfo.WriteUInt32(0); //Ice
	charInfo.WriteUInt32(0); //PhysicalBlock
	charInfo.WriteUInt32(0); //Skill
	charInfo.WriteUInt32(0); //Heroic
	charInfo.WriteUInt32(0); //Heroic_Melee
	charInfo.WriteUInt32(0); //Heroic_Ranged
	charInfo.WriteUInt32(0); //Heroic_Ranged*/
	/*End Character Defense*/

	/*Result Attack*/
	charInfo.WriteUInt16(13); //Result Attack Size
	charInfo.WriteUInt8(7); //Result Attack Flag

	charInfo.WriteUInt32(100); //AttackRange
	charInfo.WriteUInt32(50); //HitRange
	charInfo.WriteUInt32(60); //Speed

	/*charInfo.WriteUInt32(0); //AttackRange
	charInfo.WriteUInt32(0); //HitRange
	charInfo.WriteUInt32(0); //Speed
	charInfo.WriteUInt32(0); //SkillCast
	charInfo.WriteUInt32(0); //SkillDelay
	charInfo.WriteUInt32(0); //HitRate
	charInfo.WriteUInt32(0); //EvadeRate
	charInfo.WriteUInt32(0); //DodgeRate*/
	/*End Character Attack*/

	/*Character Item*/
	/*charInfo.WriteUInt32(0); //Durability
	charInfo.WriteUInt32(0); //Hand
	charInfo.WriteUInt32(0); //Rank
	charInfo.WriteUInt32(0); //Physical Rank
	charInfo.WriteUInt32(0); //MaxDurability
	charInfo.WriteUInt32(0); //Gacha*/
	/*End Character Item*/

	/*Character DIRT*/
	charInfo.WriteUInt16(9); //Result Dirt Size
	charInfo.WriteUInt8(160); //Result Dirt Flag

	charInfo.WriteUInt32(0);
	charInfo.WriteUInt32(0);
	/*charInfo.WriteUInt32(0); //Duration
	charInfo.WriteUInt32(0); //Intensity
	charInfo.WriteUInt32(0); //Range
	charInfo.WriteUInt32(0); //Target
	charInfo.WriteUInt32(0); //SkillLevel
	charInfo.WriteUInt32(0); //SkillPoint
	charInfo.WriteUInt32(0); //SkillExp
	charInfo.WriteUInt32(0); //HeroicPoint*/
	/*End Character DIRT*/

	/*Character Price*/
	/*charInfo.WriteUInt32(0); //NpcPrice
	charInfo.WriteUInt32(0); //PlayerPrice
	charInfo.WriteUInt32(0); //MoneyHigh
	charInfo.WriteUInt32(0); //MoneyLow*/
	/*End Character Price*/
	/*End Result Factor Data*/

	/*Status Factor*/
	charInfo.WriteUInt16(42); //Status Factor Size
	charInfo.WriteUInt16(3039); //Status Factor Flag

	charInfo.WriteUInt32(100); //CON
	charInfo.WriteUInt32(100); //STR
	charInfo.WriteUInt32(100); //INT
	charInfo.WriteUInt32(100); //DEX
	charInfo.WriteUInt32(100); //CHA
	charInfo.WriteUInt32(100); //WIS
	charInfo.WriteUInt32(1); //Level
	charInfo.WriteUInt32(100); //WalkSpeed
	charInfo.WriteUInt32(100); //RunSpeed
	charInfo.WriteUInt32(0); //UnionRank
	/*End Status Factor*/

	/*Type Factor*/
	charInfo.WriteUInt16(13); //Type Size
	charInfo.WriteUInt8(7); //Type Flag

	charInfo.WriteUInt32(1); //Race
	charInfo.WriteUInt32(1); //Gender
	charInfo.WriteUInt32(1); //Class
	/*End Type Factor*/

	/*Point Factor*/
	charInfo.WriteUInt16(24); //Point Size
	charInfo.WriteUInt32(31); //Point Flag

	charInfo.WriteUInt32(100); //HP
	charInfo.WriteUInt32(100); //MP
	charInfo.WriteUInt32(0); //SP
	charInfo.WriteUInt32(0); //Exp
	charInfo.WriteUInt32(0); //ExpHigh
	/*End Point Factor*/

	/*Point Max Factor*/
	charInfo.WriteUInt16(22); //Point Max Size
	charInfo.WriteUInt16(31); //Point Max Flag

	charInfo.WriteUInt32(100); //HP
	charInfo.WriteUInt32(100); //MP
	charInfo.WriteUInt32(0); //SP
	charInfo.WriteUInt32(0); //Exp
	charInfo.WriteUInt32(0); //ExpHigh
	/*End Point Max Factor*/

	/*Damage Factor*/
	charInfo.WriteUInt16(9); //Damage Size
	charInfo.WriteUInt8(3); //Damage Flag

	charInfo.WriteUInt16(2); //Damage Minimum Size
	charInfo.WriteUInt16(0); //Damage Minimum Flag

	charInfo.WriteUInt16(2); //Damage Maximum Size
	charInfo.WriteUInt16(0); //Damage Maximum Flag
	/*End Damage Factor*/

	/*Defense Factor*/
	charInfo.WriteUInt16(13); //Result Defense Size
	charInfo.WriteUInt8(3); //Result Defense Flag

	//Defense Point - Minimum
	charInfo.WriteUInt16(6); //Result Defense Minimum Size
	charInfo.WriteUInt16(1); //Result Defense Minimum Flag

	charInfo.WriteUInt32(0); //Physical

	//Defense Point - Maximum
	charInfo.WriteUInt16(2); //Result Defense Maximum Size
	charInfo.WriteUInt16(0); //Result Defense Maximum Flag
	/*End Defense Factor*/

	/*Attack Factor*/
	charInfo.WriteUInt16(13); //Result Attack Size
	charInfo.WriteUInt8(7); //Result Attack Flag

	charInfo.WriteUInt32(100); //AttackRange
	charInfo.WriteUInt32(50); //HitRange
	charInfo.WriteUInt32(60); //Speed
	/*End Attack Factor*/

	/*Dirt Factor*/
	charInfo.WriteUInt16(9); //Result Dirt Size
	charInfo.WriteUInt8(160); //Result Dirt Flag

	charInfo.WriteUInt32(0);
	charInfo.WriteUInt32(0);
	/*End Dirt Factor*/

	/*End Factor Data*/
#pragma endregion

	/*Skill Data*/
	/*charInfo.WriteUInt8(0); //ActionType

	charInfo.WriteUInt8(0); //BaseType
	charInfo.WriteUInt32(0); //BaseID

	for (int i = 0; i < 17; i++)
		charInfo.WriteUInt32(0); //ResultsFactor

	charInfo.WriteFloat(0); //PosX
	charInfo.WriteFloat(0); //PosY
	charInfo.WriteFloat(0); //PosZ

	charInfo.WriteUInt32(0); //bForceAttack
	charInfo.WriteUInt32(0); //CastDelay
	charInfo.WriteUInt32(0); //Duration
	charInfo.WriteUInt32(0); //RecastDelay
	charInfo.WriteUInt32(0); //SkillLevel
	charInfo.WriteUInt32(0); //IsFactorNotQueueing
	charInfo.WriteUInt32(0); //AdditionalEffect
	charInfo.WriteUInt16(0); //NameLength
	charInfo.WriteUInt8(0); //TargetID (Needed?)*/
	/*End SKill Data*/


	charInfo.WriteUInt64(0); //Money
	charInfo.WriteUInt64(0); //BankMoney
	charInfo.WriteUInt64(0); //CashMoney

	charInfo.WriteUInt8(0); //ActionStatus
	charInfo.WriteUInt8(0); //CriminalStatus
	
	charInfo.WriteUInt32(0); //AttackerID
	
	charInfo.WriteUInt8(0); //IsNewChar
	charInfo.WriteUInt8(0); //LastBindingIndex
	charInfo.WriteUInt8(0); //socialAnimationIndex

	charInfo.WriteUInt64(0); //specialStatus
	charInfo.WriteUInt8(0); //TransformStatus

	for(int i = 0; i < 0x20; i++)
		charInfo.WriteUInt8(0); //SkillInit [33]

	charInfo.WriteUInt8(0); //Face
	charInfo.WriteUInt8(0); //Hair

	charInfo.WriteUInt32(0); //optionFlag

	charInfo.WriteUInt8(0); //BankSize

	charInfo.WriteUInt16(0); //EventStatusFlag

	charInfo.WriteUInt32(0); //RemainCriminalTime
	charInfo.WriteUInt32(0); //RemainMurderTime

	//charInfo.WriteByteArray("Test Signature");
	for (int i = 0; i < 0x10; i++)
		charInfo.WriteUInt8(0); //Signature

	charInfo.WriteUInt16(0);
	charInfo.WriteUInt8(0);
	
	charInfo.WriteUInt32(0); //LastKilledInBattleSquare

	charInfo.ClosePacket();

	stringstream logMsg;
	logMsg << "charInfo Size:" << charInfo.getSize();
	LOG(logMsg.str());

	sock.sendPacket(&charInfo);
}