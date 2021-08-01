#include "AlefLoginServerSystems.h"

AlefServerLoginSys::AlefServerLoginSys()
{
	dbLoginSys = new AlefDBLoginSys();
	random.seed();
}

AlefServerLoginSys::~AlefServerLoginSys()
{
	delete dbLoginSys;
}

bool AlefServerLoginSys::checkLogin(string username, string password, UInt32& acctID)
{
	SharedPtr<RecordSet> rs = dbLoginSys->dbGetAccount(username, password);

	if (!rs)
		return false;

	if (rs->getTotalRowCount() == 0)
	{
		LOG("dbGetAccount() row count : 0", FATAL);
		return false;
	}
	
	if (rs->getTotalRowCount() == 1) //we should only get one value here
	{
		for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
		{
			acctID = itr->get(COLIDX(ACCOUNTCOL::ACCTID));
		}
	}
	else
		acctID = 0;

	return true;
}

bool AlefServerLoginSys::checkCharName(string charName)
{
	SharedPtr<RecordSet> rs = dbLoginSys->dbCheckCharName(charName);

	if (!rs)
		return false;

	if (rs->getTotalRowCount() != 0) //there should be no names returned for unique names
		return false;
	else
		return true;

	return false;
}

bool AlefServerLoginSys::createChar(UInt32 acctID, UInt32 tID, UInt32 face, UInt32 hair, string charName)
{
	return dbLoginSys->dbCreateCharacter(acctID, tID, face, hair, charName);
}

bool AlefServerLoginSys::deleteChar(UInt32 acctID, string charName)
{
	bool ret = dbLoginSys->dbDeleteCharacter(acctID, charName);

	SharedPtr<RecordSet> rs = dbLoginSys->dbCheckCharName(charName);

	if (!rs)
		return false;;

	if (rs->getTotalRowCount() == 0)
		ret = true; //found no character name, character succesfully deleted
	else
		ret = false; //error with deleting character

	return ret;
}

Int32 AlefServerLoginSys::generateAuthKey(UInt32 acctID)
{
	UInt32 authKey = random.next();

	if (!dbLoginSys->dbSetAuthKey(acctID, authKey))
		return -1;

	return authKey;
}

bool AlefServerLoginSys::getCharData(UInt32 acctID, clientCharDataVec& charVec)
{
	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		SharedPtr<CharacterData> charData = new CharacterData;
		SharedPtr<RecordSet> rs = dbLoginSys->dbGetCharacterFromSlot(acctID, i);

		if (!rs)
			return false;
		if (rs->getTotalRowCount() == 0)
			return true;

		for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
		{
			charData->slot = i;

			charData->charID = itr->get(COLIDX(CHARCOL::CHARID));
			charData->charName = itr->get(COLIDX(CHARCOL::CHARNAME)).toString();
			charData->charTID = itr->get(COLIDX(CHARCOL::CHARTID));
			charData->hair = itr->get(COLIDX(CHARCOL::HAIR));
			charData->face = itr->get(COLIDX(CHARCOL::FACE));

			//Position

			//I want to redo this but there isn't really a better way atm
			Int32 charLevel = itr->get(COLIDX(CHARCOL::LEVEL));
			charData->charFactors.result.status.level = charLevel;
			charData->charFactors.status.level = charLevel;

			Int32 charHP = itr->get(COLIDX(CHARCOL::HP)), 
				charMana = itr->get(COLIDX(CHARCOL::MP)), 
				charSP = itr->get(COLIDX(CHARCOL::SKILLPOINT));
			
			charData->charFactors.result.point.hp = charHP;
			charData->charFactors.result.point.mana = charMana;
			charData->charFactors.result.point.skillPoints = charSP;

			charData->charFactors.point.hp = charHP;
			charData->charFactors.point.mana = charMana;
			charData->charFactors.point.skillPoints = charSP;
		}

		if (!initStatusFactor(charData))
			return false;

		if (!initTypeFactor(charData))
			return false;

		if (!initPointMaxFactor(charData))
			return false;

		if (!initAttackFactor(charData))
			return false;

		charVec.push_back(charData);

	}

	return true;
}

bool AlefServerLoginSys::initStatusFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;

	charData->charFactors.result.status.stamina = serverDataSys->getTemplateField(TID, 11) * 100; //* levelFactor
	charData->charFactors.result.status.strength = serverDataSys->getTemplateField(TID, 12) * 100;
	charData->charFactors.result.status.intelligence = serverDataSys->getTemplateField(TID, 13) * 100;
	charData->charFactors.result.status.dexterity = serverDataSys->getTemplateField(TID, 14) * 100;
	charData->charFactors.result.status.charisma = serverDataSys->getTemplateField(TID, 15) * 100;
	charData->charFactors.result.status.luck = serverDataSys->getTemplateField(TID, 16) * 100;
	charData->charFactors.result.status.wisdom = serverDataSys->getTemplateField(TID, 17) * 100;
	charData->charFactors.result.status.moveSpeed = serverDataSys->getTemplateField(TID, 19);
	charData->charFactors.result.status.runSpeed = serverDataSys->getTemplateField(TID, 20);

	charData->charFactors.status.stamina = serverDataSys->getTemplateField(TID, 11) * 100; //* levelFactor
	charData->charFactors.status.strength = serverDataSys->getTemplateField(TID, 12) * 100;
	charData->charFactors.status.intelligence = serverDataSys->getTemplateField(TID, 13) * 100;
	charData->charFactors.status.dexterity = serverDataSys->getTemplateField(TID, 14) * 100;
	charData->charFactors.status.charisma = serverDataSys->getTemplateField(TID, 15) * 100;
	charData->charFactors.status.luck = serverDataSys->getTemplateField(TID, 16) * 100;
	charData->charFactors.status.wisdom = serverDataSys->getTemplateField(TID, 17) * 100;
	charData->charFactors.status.moveSpeed = serverDataSys->getTemplateField(TID, 19);
	charData->charFactors.status.runSpeed = serverDataSys->getTemplateField(TID, 20);

	return true;
}

bool AlefServerLoginSys::initTypeFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;

	charData->charFactors.result.type.charRace = serverDataSys->getTemplateField(TID, 23);
	charData->charFactors.result.type.charGender = serverDataSys->getTemplateField(TID, 24);
	charData->charFactors.result.type.charClass = serverDataSys->getTemplateField(TID, 25);

	charData->charFactors.type.charRace = serverDataSys->getTemplateField(TID, 23);
	charData->charFactors.type.charGender = serverDataSys->getTemplateField(TID, 24);
	charData->charFactors.type.charClass = serverDataSys->getTemplateField(TID, 25);

	return true;
}

bool AlefServerLoginSys::initPointMaxFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;

	charData->charFactors.result.pointMax.maxHP = serverDataSys->getTemplateField(TID, 28); //* levelFactor
	charData->charFactors.result.pointMax.maxMana = serverDataSys->getTemplateField(TID, 29);
	charData->charFactors.result.pointMax.maxSkillPoints = serverDataSys->getTemplateField(TID, 30);
	charData->charFactors.result.pointMax.xpLow = serverDataSys->getTemplateField(TID, 31);
	charData->charFactors.result.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	charData->charFactors.result.pointMax.AP = serverDataSys->getTemplateField(TID, 33);
	charData->charFactors.result.pointMax.MAP = serverDataSys->getTemplateField(TID, 34);
	charData->charFactors.result.pointMax.MI = serverDataSys->getTemplateField(TID, 35);
	charData->charFactors.result.pointMax.AR = serverDataSys->getTemplateField(TID, 36);
	charData->charFactors.result.pointMax.DR = serverDataSys->getTemplateField(TID, 37);

	charData->charFactors.pointMax.maxHP = serverDataSys->getTemplateField(TID, 28);
	charData->charFactors.pointMax.maxMana = serverDataSys->getTemplateField(TID, 29);
	charData->charFactors.pointMax.maxSkillPoints = serverDataSys->getTemplateField(TID, 30);
	charData->charFactors.pointMax.xpLow = serverDataSys->getTemplateField(TID, 31);
	charData->charFactors.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	charData->charFactors.pointMax.AP = serverDataSys->getTemplateField(TID, 33);
	charData->charFactors.pointMax.MAP = serverDataSys->getTemplateField(TID, 34);
	charData->charFactors.pointMax.MI = serverDataSys->getTemplateField(TID, 35);
	charData->charFactors.pointMax.AR = serverDataSys->getTemplateField(TID, 36);
	charData->charFactors.pointMax.DR = serverDataSys->getTemplateField(TID, 37);

	return true;
}

bool AlefServerLoginSys::initAttackFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;

	charData->charFactors.result.attack.range = serverDataSys->getTemplateField(TID, 64);
	charData->charFactors.result.attack.hitRange = serverDataSys->getTemplateField(TID, 65);
	charData->charFactors.result.attack.attackSpeed = serverDataSys->getTemplateField(TID, 66);

	charData->charFactors.attack.range = serverDataSys->getTemplateField(TID, 64);
	charData->charFactors.attack.hitRange = serverDataSys->getTemplateField(TID, 65);
	charData->charFactors.attack.attackSpeed = serverDataSys->getTemplateField(TID, 66);

	return true;

}