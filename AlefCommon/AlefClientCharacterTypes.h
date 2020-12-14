#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

#include <Poco/SharedPtr.h>
using Poco::SharedPtr;

#include "AlefTypes.h"

#pragma region CHARFACTORS
struct CharacterFactorStatus
{
	Int32 stamina;
	Int32 strength;
	Int32 intelligence;
	Int32 dexterity;
	Int32 charisma;
	Int32 luck;
	Int32 wisdom;
	Int32 level;
	Int32 moveSpeed;
	Int32 runSpeed;
	Int32 unionRank;
	Int32 villainPoints;
	Int32 unk;
};

struct CharacterFactorType
{
	Int32 charRace;
	Int32 charGender;
	Int32 charClass;
};

struct CharacterFactorPoint
{
	Int32 hp;
	Int32 mana;
	Int32 skillPoints;
	Int32 xpLow;
	Int32 xpHigh;
	Int32 AP;
	Int32 MAP;
	Int32 MI;
	Int32 agro;
	Int32 damage;
};

struct CharacterFactorPointMax
{
	Int32 maxHP;
	Int32 maxMana;
	Int32 maxSkillPoints;
	Int32 xpLow;
	Int32 xpHigh;
	Int32 AP;
	Int32 MAP;
	Int32 MI;
	Int32 AR;
	Int32 DR;
	Int32 MAR;
};

struct CharacterFactorRecovery
{
	Int32 hpRecover;
	Int32 manaRecovery;
	Int32 spRecovery;
};

struct CharacterFactorAttribute
{
	Int32 physical;
	Int32 magic;
	Int32 water;
	Int32 fire;
	Int32 earth;
	Int32 air;
	Int32 poison;
	Int32 lightining;
	Int32 ice;
	Int32 physBlock;
	Int32 skillBlock;
	Int32 heroic;
	Int32 heroicMelee;
	Int32 heroicRange;
	Int32 heroicMagic;
};

struct CharacterFactorDamage
{
	CharacterFactorAttribute min;
	CharacterFactorAttribute max;
};

struct CharacterFactorDefense
{
	CharacterFactorAttribute point;
	CharacterFactorAttribute rate;
};

struct CharacterFactorAttack
{
	Int32 range;
	Int32 hitRange;
	Int32 attackSpeed;
	Int32 cast;
	Int32 cooldown;
	Int32 accuracy;
	Int32 meleeDodge;
	Int32 rangeDodge;
};

struct CharacterFactorItem
{
	Int32 durability;
	Int32 hand;
	Int32 rank;
	Int32 physRank;
	Int32 maxDura;
	Int32 gacha;
};

struct CharacterFactorDirt
{
	Int32 duration;
	Int32 intensity;
	Int32 range;
	Int32 target;
	Int32 skillLevel;
	Int32 skillPoint;
	Int32 skillXP;
	Int32 heroicPoint;
};

struct CharacterFactorPrice
{
	Int32 npcPrice;
	Int32 playerPrice;
	Int32 moneyHigh;
	Int32 moneyLow;
};

struct CharacterFactorResult
{
	CharacterFactorStatus status;
	CharacterFactorType type;
	CharacterFactorPoint point;
	CharacterFactorPointMax pointMax;
	CharacterFactorRecovery recovery;
	CharacterFactorAttribute attribute;
	CharacterFactorDamage damage;
	CharacterFactorDefense defense;
	CharacterFactorAttack attack;
	CharacterFactorItem item;
	CharacterFactorDirt dirt;
	CharacterFactorPrice price;
};

struct CharacterFactors
{
	CharacterFactorResult result;
	CharacterFactorStatus status;
	CharacterFactorType type;
	CharacterFactorPoint point;
	CharacterFactorPointMax pointMax;
	CharacterFactorRecovery recovery;
	CharacterFactorAttribute attribute;
	CharacterFactorDamage damage;
	CharacterFactorDefense defense;
	CharacterFactorAttack attack;
	CharacterFactorItem item;
	CharacterFactorDirt dirt;
	CharacterFactorPrice price;
};
#pragma endregion

struct CharacterMove
{
	Alef::AlefVec3F curPos;
	Alef::AlefVec3F destPos;
	Int32 followTargetId;
	Int32 followDistance;
	float axisX;
	float axisY;
	Int8 flags;
	Int8 direction;
};

struct CharacterData
{
	string charName;
	Int32 charID;
	Int32 charTID; //Template ID
	CharacterMove charMove;
	CharacterFactors charFactors;
	UInt64 money;
	UInt64 bankMoney;
	UInt64 cashMoney;
	Int8 actionStatus;
	Int8 criminalStatus;
	Int8 face;
	Int8 hair;
	Int32 optionFlag;
	Int8 bankSize;
	UInt32 remainingCrimTime;
	UInt32 remainingMurderTime;
	string signature;
};
typedef vector<SharedPtr<CharacterData>> clientCharDataVec;

/*void AlefLoginClientLogin::sendDummyCharacter(AlefSocket& sock)
{
	Int32 i32Dummy = 100, i32One = 1, i32Zero = 0;
	float fZero = 0;
	Int8 i8Zero = 0;

	//AlefCharacter char = AlefServerCharacterSys->getCharacterData();

	//{	Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::INT32, Alef::FLOAT, Alef::FLOAT, Alef::INT8, Alef::INT8 }
	Alef::AlefVec3F pos, destPos;
	SharedPtr<AlefPacket> charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &pos, &destPos, &i32Zero, &i32Zero, &fZero, &fZero, &i8Zero, &i8Zero);
	//SharedPtr<AlefPacket> charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &char.Pos, &char.destPos, &i32Zero, &i32Zero, &fZero, &fZero, &i8Zero, &i8Zero);

#pragma region FACTORPACKETS

	//{	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 }
	SharedPtr<AlefPacket> factorStatusPkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_FACTOR_STATUS, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, &i32Dummy, &i32Dummy, &i32Dummy, &i32Dummy, 0, &i32Zero, 0);

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
	unsigned char charName[] = "test", skillInit[33] = { 0 }, signature[] = "Signature";*/
	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET,
	Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
	Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
	Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 }*/
/*SharedPtr<AlefPacket> charInfoPkt = pktInterface->buildPacket(Alef::AGPMCHARACTER_PACKET_TYPE, &i8Operation, &charID, &charTID, &nameLen, charName, &i8Zero, charMovePkt, 0, factorPkt, &i64Zero, &i64Zero, &i64Zero, &i8Zero,
	&i8Zero, &i32Zero, &i8Zero, &i8Zero, &i8Zero, &i64Zero, &i8Zero, skillInit, &i8Zero, &i8Zero, &i32Zero, &i8Zero, &i16Zero, &i32Zero,
	&i32Zero, signature, &i16Zero, 0, &i32Zero);*/