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
	CharacterFactorStatus()
	{
		stamina = 0;
		strength = 0;
		intelligence = 0;
		dexterity = 0;
		charisma = 0;
		luck = 0;
		wisdom = 0;
		level = 0;
		moveSpeed = 0;
		runSpeed = 0;
		unionRank = 0;
		villainPoints = 0;
		unk = 0;
	}
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
	CharacterFactorType()
	{
		charRace = 1;
		charGender = 1;
		charClass = 1;
	}
	Int32 charRace;
	Int32 charGender;
	Int32 charClass;
};

struct CharacterFactorPoint
{
	CharacterFactorPoint()
	{
		hp = 0;
		mana = 0;
		skillPoints = 0;
		xpLow = 0;
		xpHigh = 0;
		AP = 0;
		MAP = 0;
		MI = 0;
		agro = 0;
		damage = 0;
		damageMagic = 0;
		damageWater = 0;
		damageFire = 0;
		damageEarth = 0;
		damageAir = 0;
		damagePoison = 0;
		damageLightning = 0;
		damageIce = 0;
		bonusXP = 0;
		damageHeroic = 0;
	}
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
	Int32 damageMagic;
	Int32 damageWater;
	Int32 damageFire;
	Int32 damageEarth;
	Int32 damageAir;
	Int32 damagePoison;
	Int32 damageLightning;
	Int32 damageIce;
	Int32 bonusXP;
	Int32 damageHeroic;
};

struct CharacterFactorPointMax
{
	CharacterFactorPointMax()
	{
		maxHP = 0;
		maxMana = 0;
		maxSkillPoints = 0;
		xpLow = 0;
		xpHigh = 0;
		AP = 0;
		MAP = 0;
		MI = 0;
		AR = 0;
		DR = 0;
		MAR = 0;
	}
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
	CharacterFactorRecovery()
	{
		hpRecover = 0;
		manaRecovery = 0;
		spRecovery = 0;
	}
	Int32 hpRecover;
	Int32 manaRecovery;
	Int32 spRecovery;
};

struct CharacterFactorAttribute
{
	CharacterFactorAttribute()
	{
		physical = 0;
		magic = 0;
		water = 0;
		fire = 0;
		earth = 0;
		air = 0;
		poison = 0;
		lightning = 0;
		ice = 0;
		physBlock = 0;
		skillBlock = 0;
		heroic = 0;
		heroicMelee = 0;
		heroicRange = 0;
		heroicMagic = 0;
	}
	Int32 physical;
	Int32 magic;
	Int32 water;
	Int32 fire;
	Int32 earth;
	Int32 air;
	Int32 poison;
	Int32 lightning;
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
	CharacterFactorAttack()
	{
		range = 0;
		hitRange = 0;
		attackSpeed = 0;
		cast = 0;
		cooldown = 0;
		accuracy = 0;
		meleeDodge = 0;
		rangeDodge = 0;
	}
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
	CharacterFactorItem()
	{
		durability = 0;
		hand = 0;
		rank = 0;
		physRank = 0;
		maxDura = 0;
		gacha = 0;
	}
	Int32 durability;
	Int32 hand;
	Int32 rank;
	Int32 physRank;
	Int32 maxDura;
	Int32 gacha;
};

struct CharacterFactorDirt
{
	CharacterFactorDirt()
	{
		duration = 0;
		intensity = 0;
		range = 0;
		target = 0;
		skillLevel = 0;
		skillPoint = 0;
		skillXP = 0;
		heroicPoint = 0;
	}
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
	CharacterFactorPrice()
	{
		npcPrice = 0;
		playerPrice = 0;
		moneyHigh = 0;
		moneyLow = 0;
	}
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
	CharacterMove()
	{
		curPos = { 0, 0, 0 };
		destPos = { 0, 0, 0 };
		followTargetId = 0;
		followDistance = 0;
		axisX = 0;
		axisY = 0;
		flags = 0;
		direction = 0;
	}
	Alef::AlefVec3F curPos;
	Alef::AlefVec3F destPos;
	Int32 followTargetId;
	Int32 followDistance;
	float axisX;
	float axisY;
	Int8 flags;
	Int8 direction;
};

struct CharacterAction
{

};

struct CharacterData
{
	CharacterData()
	{
		charID = 0;
		charSessionID = 0;
		charTID = 0;
		charName = "";
		charStatus = 0;
		money = 0;
		bankMoney = 0;
		cashMoney = 0;
		actionStatus = 0;
		criminalStatus = 0;
		attackerID = 0;
		isNewChar = 0;
		socialAnimIdx = 0;
		lastBindingIdx = 0;
		specialStatus = 0;
		transformStatus = 0;
		skillInit.reserve(33);
		face = 0;
		hair = 0;
		optionFlag = 0;
		bankSize = 0;
		eventStatusFlag = 0;
		remainingCrimTime = 0;
		remainingMurderTime = 0;
		signature = "";
		GGSize = 0;
		lastKilledBSquare = 0;
	}
	UInt32 slot;

	UInt32 charID;
	UInt32 charSessionID;
	UInt32 charTID; //Template ID
	string charName;
	UInt8 charStatus;
	CharacterMove charMove;
	CharacterAction charAction;
	CharacterFactors charFactors;
	UInt64 money;
	UInt64 bankMoney;
	UInt64 cashMoney;
	UInt8 actionStatus;
	UInt8 criminalStatus;
	UInt32 attackerID;
	UInt8 isNewChar;
	UInt8 socialAnimIdx;
	UInt8 lastBindingIdx;
	UInt64 specialStatus;
	UInt8 transformStatus;
	string skillInit;
	UInt8 face;
	UInt8 hair;
	UInt32 optionFlag;
	UInt8 bankSize;
	UInt16 eventStatusFlag;
	UInt32 remainingCrimTime;
	UInt32 remainingMurderTime;
	string signature;
	UInt16 GGSize;
	UInt32 lastKilledBSquare;
};
typedef vector<SharedPtr<CharacterData>> clientCharDataVec;