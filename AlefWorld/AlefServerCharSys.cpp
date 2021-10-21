#include "AlefWorldServerSystems.h"

constexpr auto SINGLEROW = 0; //For single 0th row values;

AlefServerCharSys::AlefServerCharSys()
{
	dbCharSys = new AlefDBCharSys();
}

AlefServerCharSys::~AlefServerCharSys()
{
	delete dbCharSys;
}

UInt32 AlefServerCharSys::getAcctID(Int32 authKey)
{
	UInt32 acctID = -1;
	SharedPtr<RecordSet> rs = dbCharSys->dbGetAcctID(authKey);

	if (!rs)
		return 0;
	if (rs->getTotalRowCount() != 1) //We should only get one row returned
		return 0;

	acctID = rs->row(SINGLEROW).get(COLIDX(ACCOUNTCOL::ACCTID));

	return acctID;
}

CharacterData AlefServerCharSys::getCharacter(UInt32 acctID, string& charName)
{
	string cleanName = charName.substr(0, charName.find("#", 0));
	CharacterData charData;
	charData.charID = -1;

	SharedPtr<RecordSet> rs = dbCharSys->dbGetChara(acctID, cleanName);

	if (!rs)
		return charData;
	if (rs->getTotalRowCount() != 1) //we should only get one character given to us
		return charData;

	for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
	{
		charData.slot = 1; //current character slot will always be 1

		charData.charID = itr->get(COLIDX(CHARCOL::CHARID));
		charData.charName = itr->get(COLIDX(CHARCOL::CHARNAME)).toString();
		charData.charTID = itr->get(COLIDX(CHARCOL::CHARTID));
		charData.hair = itr->get(COLIDX(CHARCOL::HAIR));
		charData.face = itr->get(COLIDX(CHARCOL::FACE));

		//Position
		charData.charMove.curPos.x = itr->get(COLIDX(CHARCOL::POSX));
		charData.charMove.curPos.y = itr->get(COLIDX(CHARCOL::POSY));
		charData.charMove.curPos.z = itr->get(COLIDX(CHARCOL::POSZ));

		//I want to redo this but there isn't really a better way atm
		Int32 charLevel = itr->get(COLIDX(CHARCOL::LEVEL));
		charData.charFactors.result.status.level = charLevel;
		charData.charFactors.status.level = charLevel;

		Int32 charHP = itr->get(COLIDX(CHARCOL::HP)),
			charMana = itr->get(COLIDX(CHARCOL::MP)),
			charSP = itr->get(COLIDX(CHARCOL::SKILLPOINT));

		charData.charFactors.result.point.hp = charHP;
		charData.charFactors.result.point.mana = charMana;
		charData.charFactors.result.point.skillPoints = charSP;

		charData.charFactors.point.hp = charHP;
		charData.charFactors.point.mana = charMana;
		charData.charFactors.point.skillPoints = charSP;
	}

	initStatusFactor(charData);
	initTypeFactor(charData);
	initPointMaxFactor(charData);
	initAttackFactor(charData);

	return charData;
}

void AlefServerCharSys::initStatusFactor(CharacterData& charData)
{
	UInt32 TID = charData.charTID;

	charData.charFactors.result.status.stamina = serverDataSys->getTemplateField(TID, 11) * 100; //* levelFactor
	charData.charFactors.result.status.strength = serverDataSys->getTemplateField(TID, 12) * 100;
	charData.charFactors.result.status.intelligence = serverDataSys->getTemplateField(TID, 13) * 100;
	charData.charFactors.result.status.dexterity = serverDataSys->getTemplateField(TID, 14) * 100;
	charData.charFactors.result.status.charisma = serverDataSys->getTemplateField(TID, 15) * 100;
	charData.charFactors.result.status.luck = serverDataSys->getTemplateField(TID, 16) * 100;
	charData.charFactors.result.status.wisdom = serverDataSys->getTemplateField(TID, 17) * 100;
	charData.charFactors.result.status.moveSpeed = serverDataSys->getTemplateField(TID, 19);
	charData.charFactors.result.status.runSpeed = serverDataSys->getTemplateField(TID, 20);

	charData.charFactors.status.stamina = serverDataSys->getTemplateField(TID, 11) * 100; //* levelFactor
	charData.charFactors.status.strength = serverDataSys->getTemplateField(TID, 12) * 100;
	charData.charFactors.status.intelligence = serverDataSys->getTemplateField(TID, 13) * 100;
	charData.charFactors.status.dexterity = serverDataSys->getTemplateField(TID, 14) * 100;
	charData.charFactors.status.charisma = serverDataSys->getTemplateField(TID, 15) * 100;
	charData.charFactors.status.luck = serverDataSys->getTemplateField(TID, 16) * 100;
	charData.charFactors.status.wisdom = serverDataSys->getTemplateField(TID, 17) * 100;
	charData.charFactors.status.moveSpeed = serverDataSys->getTemplateField(TID, 19);
	charData.charFactors.status.runSpeed = serverDataSys->getTemplateField(TID, 20);
}

void AlefServerCharSys::initTypeFactor(CharacterData& charData)
{
	UInt32 TID = charData.charTID;

	charData.charFactors.result.type.charRace = serverDataSys->getTemplateField(TID, 23);
	charData.charFactors.result.type.charGender = serverDataSys->getTemplateField(TID, 24);
	charData.charFactors.result.type.charClass = serverDataSys->getTemplateField(TID, 25);

	charData.charFactors.type.charRace = serverDataSys->getTemplateField(TID, 23);
	charData.charFactors.type.charGender = serverDataSys->getTemplateField(TID, 24);
	charData.charFactors.type.charClass = serverDataSys->getTemplateField(TID, 25);
}

void AlefServerCharSys::initPointMaxFactor(CharacterData& charData)
{
	UInt32 TID = charData.charTID;

	charData.charFactors.result.pointMax.maxHP = serverDataSys->getTemplateField(TID, 28); //* levelFactor
	charData.charFactors.result.pointMax.maxMana = serverDataSys->getTemplateField(TID, 29);
	charData.charFactors.result.pointMax.maxSkillPoints = serverDataSys->getTemplateField(TID, 30);
	charData.charFactors.result.pointMax.xpLow = serverDataSys->getTemplateField(TID, 31);
	charData.charFactors.result.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	charData.charFactors.result.pointMax.AP = serverDataSys->getTemplateField(TID, 33);
	charData.charFactors.result.pointMax.MAP = serverDataSys->getTemplateField(TID, 34);
	charData.charFactors.result.pointMax.MI = serverDataSys->getTemplateField(TID, 35);
	charData.charFactors.result.pointMax.AR = serverDataSys->getTemplateField(TID, 36);
	charData.charFactors.result.pointMax.DR = serverDataSys->getTemplateField(TID, 37);

	charData.charFactors.pointMax.maxHP = serverDataSys->getTemplateField(TID, 28);
	charData.charFactors.pointMax.maxMana = serverDataSys->getTemplateField(TID, 29);
	charData.charFactors.pointMax.maxSkillPoints = serverDataSys->getTemplateField(TID, 30);
	charData.charFactors.pointMax.xpLow = serverDataSys->getTemplateField(TID, 31);
	charData.charFactors.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	charData.charFactors.pointMax.AP = serverDataSys->getTemplateField(TID, 33);
	charData.charFactors.pointMax.MAP = serverDataSys->getTemplateField(TID, 34);
	charData.charFactors.pointMax.MI = serverDataSys->getTemplateField(TID, 35);
	charData.charFactors.pointMax.AR = serverDataSys->getTemplateField(TID, 36);
	charData.charFactors.pointMax.DR = serverDataSys->getTemplateField(TID, 37);
}

void AlefServerCharSys::initAttackFactor(CharacterData& charData)
{
	UInt32 TID = charData.charTID;

	charData.charFactors.result.attack.range = serverDataSys->getTemplateField(TID, 64);
	charData.charFactors.result.attack.hitRange = serverDataSys->getTemplateField(TID, 65);
	charData.charFactors.result.attack.attackSpeed = serverDataSys->getTemplateField(TID, 66);

	charData.charFactors.attack.range = serverDataSys->getTemplateField(TID, 64);
	charData.charFactors.attack.hitRange = serverDataSys->getTemplateField(TID, 65);
	charData.charFactors.attack.attackSpeed = serverDataSys->getTemplateField(TID, 66);
}
