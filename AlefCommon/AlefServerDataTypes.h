#pragma once

#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;
using std::string;

#include <Poco/Dynamic/Var.h>
#include <Poco/SharedPtr.h>
#include <Poco/HashMap.h>
using Poco::Dynamic::Var;
using Poco::SharedPtr;
using Poco::HashMap;

struct TemplateFieldInfo
{
	int fieldID;
	string fieldName;
	int fieldType;
};
typedef vector<SharedPtr<TemplateFieldInfo>> charTemplateFieldVec;

struct TemplateField
{
	int fieldID;
	Var fieldValue;
};

struct TemplateData
{
	int templateID;
	vector<SharedPtr<TemplateField>> templateFields;
};
typedef vector<SharedPtr<TemplateData>> charTemplateVec;

struct CharDataInfo
{
	UInt32 ID; //DB ID
	UInt32 tID; //template ID
	string name;
	UInt32 type;
	UInt32 race;
	UInt32 gender;
	UInt32 clss; //class
	UInt32 LVL;
	UInt32 STR;
	UInt32 DEX;
	UInt32 INT;
	UInt32 CON;
	UInt32 CHA;
	UInt32 WIS;
	UInt32 AC;
	UInt32 MAXHP;
	UInt32 MAXSP;
	UInt32 MAXMP;
	UInt32 CURHP;
	UInt32 CURSP;
	UInt32 CURMP;
	UInt32 walkSpeed;
	UInt32 runSpeed;
	UInt32 AR;
	UInt32 DR;
	UInt32 MAR;
	UInt32 MDR;
	UInt32 minDmg;
	UInt32 maxDmg;
	UInt32 atkSpeed;
	UInt32 range;
	UInt32 fireRes;
	UInt32 waterRes;
	UInt32 airRes;
	UInt32 earthRes;
	UInt32 magicRes;
	UInt32 poisonRes;
	UInt32 iceRes;
	UInt32 thunderRes;
	UInt32 magicHistory;
	UInt32 murderPoint;
	UInt32 exp;
	UInt32 fireMinDmg;
	UInt32 fireMaxDmg;
	UInt32 waterMinDmg;
	UInt32 waterMaxDmg;
	UInt32 earthMinDmg;
	UInt32 earthMaxDmg;
	UInt32 airMinDmg;
	UInt32 airMaxDmg;
	UInt32 magicMinDmg;
	UInt32 magicMaxDmg;
	UInt32 poisonMinDmg;
	UInt32 poisonMaxDmg;
	UInt32 iceMinDmg;
	UInt32 iceMaxDmg;
	UInt32 thunderMinDmg;
	UInt32 thunderMaxDmg;
	UInt32 physResis;
	UInt32 skillBlock;
	UInt32 sight;
	UInt32 badge1RankPoint;
	UInt32 badge2RankPoint;
	UInt32 badge3RankPoint;
	UInt32 rank;
	UInt32 itemPoint;
	UInt32 guild;
	UInt32 guildRank;
	UInt32 guildScore;
	UInt32 lostExp;
	UInt32 weapon;
	string item1;
	string item2;
	string item3;
	string item4;
	UInt32 armorHead;
	UInt32 armorBody;
	UInt32 armorArm;
	UInt32 armorHand;
	UInt32 armorLeg;
	UInt32 armorFoot;
	UInt32 armorMantle;
	UInt32 money;
	UInt32 geldMin;
	UInt32 geldMax;
	UInt32 dropTID;
	UInt32 slowPercent;
	UInt32 slowTime;
	UInt32 fastPercent;
	UInt32 fastTime;
	UInt32 productSkinName;
	UInt32 productMeat;
	UInt32 productMeatName;
	UInt32 productGarbage;
	UInt32 productGarbageName;
	UInt32 attributeType;
	string nameColor;
	UInt32 dropRank;
	float dropMeditation;
	UInt32 spiritType;
	UInt32 landAttachType;
	UInt32 tameable;
	UInt32 runCorrect;
	UInt32 attackCorrect;
	UInt32 siegeCharType;
	UInt32 rangeType;
	UInt32 stamPoint;
	UInt32 petType;
	UInt32 startStam;
	UInt32 heroicMinDmg;
	UInt32 heroicMaxDmg;
	UInt32 heroicDefPt;
};
typedef vector<SharedPtr<CharDataInfo>> charDataVec;