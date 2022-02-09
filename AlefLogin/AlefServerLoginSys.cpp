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

		if (!initCharFactors(charData))
			return false;		

		charVec.push_back(charData);

	}

	return true;
}

bool AlefServerLoginSys::initCharFactors(SharedPtr<CharacterData>& charData)
{
	if (!initStatusFactor(charData))
		return false;

	if (!initTypeFactor(charData))
		return false;

	if (!initPointMaxFactor(charData))
		return false;

	if (!initAttackFactor(charData))
		return false;
}

bool AlefServerLoginSys::initStatusFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;
	SharedPtr<CharDataInfo> serverChar = serverDataSys->getCharDataFromTemplID(TID);

	if (serverChar.isNull())
		return false;
	

	charData->charFactors.result.status.stamina = serverChar->CON * 100; //* levelFactor
	charData->charFactors.result.status.strength = serverChar->STR * 100;
	charData->charFactors.result.status.intelligence = serverChar->INT * 100;
	charData->charFactors.result.status.dexterity = serverChar->DEX * 100;
	charData->charFactors.result.status.charisma = serverChar->CHA * 100;
	charData->charFactors.result.status.luck = 0;
	charData->charFactors.result.status.wisdom = serverChar->WIS * 100;
	charData->charFactors.result.status.moveSpeed = serverChar->walkSpeed;
	charData->charFactors.result.status.runSpeed = serverChar->runSpeed;

	charData->charFactors.status.stamina = serverChar->CON * 100; //* levelFactor
	charData->charFactors.status.strength = serverChar->STR * 100;
	charData->charFactors.status.intelligence = serverChar->INT * 100;
	charData->charFactors.status.dexterity = serverChar->DEX * 100;
	charData->charFactors.status.charisma = serverChar->CHA * 100;
	charData->charFactors.status.luck = 0;
	charData->charFactors.status.wisdom = serverChar->WIS * 100;
	charData->charFactors.status.moveSpeed = serverChar->walkSpeed;
	charData->charFactors.status.runSpeed = serverChar->runSpeed;

	return true;
}

bool AlefServerLoginSys::initTypeFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;
	SharedPtr<CharDataInfo> serverChar = serverDataSys->getCharDataFromTemplID(TID);

	if (serverChar.isNull())
		return false;

	charData->charFactors.result.type.charRace = serverChar->race;
	charData->charFactors.result.type.charGender = serverChar->gender;
	charData->charFactors.result.type.charClass = serverChar->clss;

	charData->charFactors.type.charRace = serverChar->race;
	charData->charFactors.type.charGender = serverChar->gender;
	charData->charFactors.type.charClass = serverChar->clss;

	return true;
}

bool AlefServerLoginSys::initPointMaxFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;

	SharedPtr<CharDataInfo> serverChar = serverDataSys->getCharDataFromTemplID(TID);

	if (serverChar.isNull())
		return false;

	charData->charFactors.result.pointMax.maxHP = serverChar->MAXHP; //* levelFactor
	charData->charFactors.result.pointMax.maxMana = serverChar->MAXMP;
	charData->charFactors.result.pointMax.maxSkillPoints = serverChar->MAXSP;
	//charData->charFactors.result.pointMax.xpLow = serverChar->exp;
	//charData->charFactors.result.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	//charData->charFactors.result.pointMax.AP = serverChar->;
	charData->charFactors.result.pointMax.MAP = serverChar->MAR; //Is this correct?
	//charData->charFactors.result.pointMax.MI = serverChar->;
	charData->charFactors.result.pointMax.AR = serverChar->AR;
	charData->charFactors.result.pointMax.DR = serverChar->DR;

	charData->charFactors.pointMax.maxHP = serverChar->MAXHP; //* levelFactor
	charData->charFactors.pointMax.maxMana = serverChar->MAXMP;
	charData->charFactors.pointMax.maxSkillPoints = serverChar->MAXSP;
	//charData->charFactors.pointMax.xpLow = serverChar->exp;
	//charData->charFactors.pointMax.xpHigh = serverDataSys->getTemplateField(TID, 32);
	//charData->charFactors.pointMax.AP = serverChar->;
	charData->charFactors.pointMax.MAP = serverChar->MAR; //Is this correct?
	//charData->charFactors.pointMax.MI = serverChar->;
	charData->charFactors.pointMax.AR = serverChar->AR;
	charData->charFactors.pointMax.DR = serverChar->DR;

	return true;
}

bool AlefServerLoginSys::initAttackFactor(SharedPtr<CharacterData>& charData)
{
	UInt32 TID = charData->charTID;
	SharedPtr<CharDataInfo> serverChar = serverDataSys->getCharDataFromTemplID(TID);

	if (serverChar.isNull())
		return false;

	charData->charFactors.result.attack.range = serverChar->range;
	//charData->charFactors.result.attack.hitRange = serverChar->hit;
	charData->charFactors.result.attack.attackSpeed = serverChar->atkSpeed;

	charData->charFactors.attack.range = serverChar->range;
	//charData->charFactors.attack.hitRange = serverDataSys->getTemplateField(TID, 65);
	charData->charFactors.attack.attackSpeed = serverChar->atkSpeed;

	return true;

}