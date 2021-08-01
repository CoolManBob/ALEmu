#include "AlefServerDataSys.h"

AlefServerDataSys::AlefServerDataSys()
{
	dbDataSys = new AlefDBDataSys();
}

AlefServerDataSys::~AlefServerDataSys()
{
	delete dbDataSys;
	templateFieldVec.clear();
	templateDataVec.clear();
	charDataTableVec.clear();
}

bool AlefServerDataSys::initData()
{
	if (!initTemplates())
		return false;

	if (!initCharDataTable())
		return false;

	if (!initCharLoginPosTable())
		return false;

	return true;
}

bool AlefServerDataSys::initTemplates()
{
	LOG("Loading Entity Templates...", WARNING);
	SharedPtr<RecordSet> rs = dbDataSys->dbGetTemplateFields();

	if (!rs)
		return false;

	if (rs->getTotalRowCount() == 0)
	{
		LOG("dbGetTemplateFields() row count : 0", FATAL);
		return false;
	}

	for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
	{
		SharedPtr<TemplateFieldInfo> tempField = new TemplateFieldInfo;
		tempField->fieldID = itr->get(COLIDX(TEMPLATEFIELDCOL::ID));
		tempField->fieldName = itr->get(COLIDX(TEMPLATEFIELDCOL::NAME)).toString();
		tempField->fieldType = itr->get(COLIDX(TEMPLATEFIELDCOL::TYPE));

		templateFieldVec.push_back(tempField);
	}

	rs = dbDataSys->dbGetFullTemplateData();

	if (rs->getTotalRowCount() == 0)
	{
		LOG("dbGetFullTemplateData() row count : 0", FATAL);
		return false;
	}

	//Parsing code assumes template data to be stored in sequential order
	int currentTemplate = 0;
	SharedPtr<TemplateData> tempData;
	SharedPtr<TemplateField> tempField;
	for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
	{
		if (currentTemplate == itr->get(COLIDX(TEMPLATEDATACOL::TID)))
		{
			if (tempData.isNull())
				return false; //tempData is NULL for some reason so we raise an error.

			tempField = new TemplateField;
			tempField->fieldID = itr->get(COLIDX(TEMPLATEDATACOL::FIELDID));
			tempField->fieldValue = itr->get(COLIDX(TEMPLATEDATACOL::FIELDVAL));

			tempData->templateFields.push_back(tempField);
		}
		else
		{
			if (!tempData.isNull()) //Transition from one template id to the next so we add it.
				templateDataVec.push_back(tempData);

			currentTemplate = itr->get(COLIDX(TEMPLATEDATACOL::TID));
			tempData = new TemplateData;
			tempData->templateID = itr->get(COLIDX(TEMPLATEDATACOL::TID));

			tempField = new TemplateField;
			tempField->fieldID = itr->get(COLIDX(TEMPLATEDATACOL::FIELDID));
			tempField->fieldValue = itr->get(COLIDX(TEMPLATEDATACOL::FIELDVAL));

			tempData->templateFields.push_back(tempField);
		}
	}

	if (!tempData.isNull())
		templateDataVec.push_back(tempData); //This will push the final parsed template, we must do this due to the logic of the parsing code.

	LOG("Entity Templates Loaded!", WARNING);

	return true;
}

bool AlefServerDataSys::initCharDataTable()
{
	LOG("Loading Entity Data Table...", WARNING);

	SharedPtr<RecordSet> rs = dbDataSys->dbGetCharDataTable();

	if (!rs)
		return false;

	if (rs->getTotalRowCount() == 0)
	{
		LOG("dbGetCharDataTable() row count : 0", FATAL);
		return false;
	}

	for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
	{
		SharedPtr<CharDataInfo> charData = new CharDataInfo;
		charData->ID = itr->get(COLIDX(CHARDATACOL::ID));
		charData->tID = itr->get(COLIDX(CHARDATACOL::TID));
		charData->name = itr->get(COLIDX(CHARDATACOL::NAME)).toString();
		charData->type = itr->get(COLIDX(CHARDATACOL::TYPE));
		charData->race = itr->get(COLIDX(CHARDATACOL::RACE));
		charData->gender = itr->get(COLIDX(CHARDATACOL::GENDER));
		charData->clss = itr->get(COLIDX(CHARDATACOL::CLASS));
		charData->LVL = itr->get(COLIDX(CHARDATACOL::LVL));
		charData->STR = itr->get(COLIDX(CHARDATACOL::STR));
		charData->DEX = itr->get(COLIDX(CHARDATACOL::DEX));
		charData->INT = itr->get(COLIDX(CHARDATACOL::INT));
		charData->CON = itr->get(COLIDX(CHARDATACOL::CON));
		charData->CHA = itr->get(COLIDX(CHARDATACOL::CHA));
		charData->WIS = itr->get(COLIDX(CHARDATACOL::WIS));
		charData->AC = itr->get(COLIDX(CHARDATACOL::AC));
		charData->MAXHP = itr->get(COLIDX(CHARDATACOL::MAXHP));
		charData->MAXSP = itr->get(COLIDX(CHARDATACOL::MAXSP));
		charData->MAXMP = itr->get(COLIDX(CHARDATACOL::MAXMP));
		charData->CURHP = itr->get(COLIDX(CHARDATACOL::CURHP));
		charData->CURSP = itr->get(COLIDX(CHARDATACOL::CURSP));
		charData->CURMP = itr->get(COLIDX(CHARDATACOL::CURMP));
		charData->walkSpeed = itr->get(COLIDX(CHARDATACOL::WALKSPEED));
		charData->runSpeed = itr->get(COLIDX(CHARDATACOL::RUNSPEED));
		charData->AR = itr->get(COLIDX(CHARDATACOL::AR));
		charData->DR = itr->get(COLIDX(CHARDATACOL::DR));
		charData->MAR = itr->get(COLIDX(CHARDATACOL::MAR));
		charData->MDR = itr->get(COLIDX(CHARDATACOL::MDR));
		charData->minDmg = itr->get(COLIDX(CHARDATACOL::MINDMG));
		charData->maxDmg = itr->get(COLIDX(CHARDATACOL::MAXDMG));
		charData->atkSpeed = itr->get(COLIDX(CHARDATACOL::ATKSPEED));
		charData->range = itr->get(COLIDX(CHARDATACOL::RANGE));
		charData->fireRes = itr->get(COLIDX(CHARDATACOL::FIRERES));
		charData->waterRes = itr->get(COLIDX(CHARDATACOL::WATERRES));
		charData->airRes = itr->get(COLIDX(CHARDATACOL::AIRRES));
		charData->earthRes = itr->get(COLIDX(CHARDATACOL::EARTHRES));
		charData->magicRes = itr->get(COLIDX(CHARDATACOL::MAGICRES));
		charData->poisonRes = itr->get(COLIDX(CHARDATACOL::POISONRES));
		charData->iceRes = itr->get(COLIDX(CHARDATACOL::ICERES));
		charData->thunderRes = itr->get(COLIDX(CHARDATACOL::THUNDERRES));
		charData->magicHistory = itr->get(COLIDX(CHARDATACOL::MAGICHISTORY));
		charData->murderPoint = itr->get(COLIDX(CHARDATACOL::MURDERPOINT));
		charData->exp = itr->get(COLIDX(CHARDATACOL::EXP));
		charData->fireMinDmg = itr->get(COLIDX(CHARDATACOL::FIREMINDMG));
		charData->fireMaxDmg = itr->get(COLIDX(CHARDATACOL::FIREMAXDMG));
		charData->waterMinDmg = itr->get(COLIDX(CHARDATACOL::WATERMINDMG));
		charData->waterMaxDmg = itr->get(COLIDX(CHARDATACOL::WATERMAXDMG));
		charData->earthMinDmg = itr->get(COLIDX(CHARDATACOL::EARTHMINDMG));
		charData->earthMaxDmg = itr->get(COLIDX(CHARDATACOL::EARTHMAXDMG));
		charData->airMinDmg = itr->get(COLIDX(CHARDATACOL::AIRMINDMG));
		charData->airMaxDmg = itr->get(COLIDX(CHARDATACOL::AIRMAXDMG));
		charData->magicMinDmg = itr->get(COLIDX(CHARDATACOL::MAGICMINDMG));
		charData->magicMaxDmg = itr->get(COLIDX(CHARDATACOL::MAGICMAXDMG));
		charData->poisonMinDmg = itr->get(COLIDX(CHARDATACOL::POISONMINDMG));
		charData->poisonMaxDmg = itr->get(COLIDX(CHARDATACOL::POISONMAXDMG));
		charData->iceMinDmg = itr->get(COLIDX(CHARDATACOL::ICEMINDMG));
		charData->iceMaxDmg = itr->get(COLIDX(CHARDATACOL::ICEMAXDMG));
		charData->thunderMinDmg = itr->get(COLIDX(CHARDATACOL::THUNDERMINDMG));
		charData->thunderMaxDmg = itr->get(COLIDX(CHARDATACOL::THUNDERMAXDMG));
		charData->physResis = itr->get(COLIDX(CHARDATACOL::PHYSRESIS));
		charData->skillBlock = itr->get(COLIDX(CHARDATACOL::SKILLBLOCK));
		charData->sight = itr->get(COLIDX(CHARDATACOL::SIGHT));
		charData->badge1RankPoint = itr->get(COLIDX(CHARDATACOL::BADGE1RANKPOINT));
		charData->badge2RankPoint = itr->get(COLIDX(CHARDATACOL::BADGE2RANKPOINT));
		charData->badge3RankPoint = itr->get(COLIDX(CHARDATACOL::BADGE3RANKPOINT));
		charData->rank = itr->get(COLIDX(CHARDATACOL::RANK));
		charData->itemPoint = itr->get(COLIDX(CHARDATACOL::ITEMPOINT));
		charData->guild = itr->get(COLIDX(CHARDATACOL::GUILD));
		charData->guildRank = itr->get(COLIDX(CHARDATACOL::GUILDRANK));
		charData->guildScore = itr->get(COLIDX(CHARDATACOL::GUILDSCORE));
		charData->lostExp = itr->get(COLIDX(CHARDATACOL::LOSTEXP));
		charData->weapon = itr->get(COLIDX(CHARDATACOL::WEAPON));
		charData->item1 = itr->get(COLIDX(CHARDATACOL::ITEM1)).toString();
		charData->item2 = itr->get(COLIDX(CHARDATACOL::ITEM2)).toString();
		charData->item3 = itr->get(COLIDX(CHARDATACOL::ITEM3)).toString();
		charData->item4 = itr->get(COLIDX(CHARDATACOL::ITEM4)).toString();
		charData->armorHead = itr->get(COLIDX(CHARDATACOL::ARMORHEAD));
		charData->armorBody = itr->get(COLIDX(CHARDATACOL::ARMORBODY));
		charData->armorArm = itr->get(COLIDX(CHARDATACOL::ARMORARM));
		charData->armorHand = itr->get(COLIDX(CHARDATACOL::ARMORHAND));
		charData->armorLeg = itr->get(COLIDX(CHARDATACOL::ARMORLEG));
		charData->armorFoot = itr->get(COLIDX(CHARDATACOL::ARMORFOOT));
		charData->armorMantle = itr->get(COLIDX(CHARDATACOL::ARMORMANTLE));
		charData->money = itr->get(COLIDX(CHARDATACOL::MONEY));
		charData->geldMin = itr->get(COLIDX(CHARDATACOL::GELDMIN));
		charData->geldMax = itr->get(COLIDX(CHARDATACOL::GELDMAX));
		charData->dropTID = itr->get(COLIDX(CHARDATACOL::DROPTID));
		charData->slowPercent = itr->get(COLIDX(CHARDATACOL::SLOWPERCENT));
		charData->slowTime = itr->get(COLIDX(CHARDATACOL::SLOWTIME));
		charData->fastPercent = itr->get(COLIDX(CHARDATACOL::FASTPERCENT));
		charData->fastTime = itr->get(COLIDX(CHARDATACOL::FASTTIME));
		charData->productSkinName = itr->get(COLIDX(CHARDATACOL::PRODUCTSKINNAME));
		charData->productMeat = itr->get(COLIDX(CHARDATACOL::PRODUCTMEAT));
		charData->productMeatName = itr->get(COLIDX(CHARDATACOL::PRODUCTMEATNAME));
		charData->productGarbage = itr->get(COLIDX(CHARDATACOL::PRODUCTGARBAGE));
		charData->productGarbageName = itr->get(COLIDX(CHARDATACOL::PRODUCTGARBAGENAME));
		charData->attributeType = itr->get(COLIDX(CHARDATACOL::ATTRIBUTETYPE));
		charData->nameColor = itr->get(COLIDX(CHARDATACOL::NAMECOLOR)).toString();
		charData->dropRank = itr->get(COLIDX(CHARDATACOL::DROPRANK));
		charData->dropMeditation = itr->get(COLIDX(CHARDATACOL::DROPMEDITATION));
		charData->spiritType = itr->get(COLIDX(CHARDATACOL::SPIRITTYPE));
		charData->landAttachType = itr->get(COLIDX(CHARDATACOL::LANDATTACHTYPE));
		charData->tameable = itr->get(COLIDX(CHARDATACOL::TAMEABLE));
		charData->runCorrect = itr->get(COLIDX(CHARDATACOL::RUNCORRECT));
		charData->attackCorrect = itr->get(COLIDX(CHARDATACOL::ATTACKCORRECT));
		charData->siegeCharType = itr->get(COLIDX(CHARDATACOL::SIEGECHARTYPE));
		charData->rangeType = itr->get(COLIDX(CHARDATACOL::RANGETYPE));
		charData->stamPoint = itr->get(COLIDX(CHARDATACOL::STAMPOINT));
		charData->petType = itr->get(COLIDX(CHARDATACOL::PETTYPE));
		charData->startStam = itr->get(COLIDX(CHARDATACOL::STARTSTAM));
		charData->heroicMinDmg = itr->get(COLIDX(CHARDATACOL::HEROICMINDMG));
		charData->heroicMaxDmg = itr->get(COLIDX(CHARDATACOL::HEROICMAXDMG));
		charData->heroicDefPt = itr->get(COLIDX(CHARDATACOL::HEROICDEFPT));

		charDataTableVec.push_back(charData);
	}

	LOG("Entity Data Table Loaded!", WARNING);

	return true;
}

bool AlefServerDataSys::initCharLoginPosTable()
{
	LOG("Loading BaseChar LoginPos Data Table...", WARNING);

	SharedPtr<RecordSet> rs = dbDataSys->dbGetLoginCharPos();

	if (!rs)
		return false;

	if (rs->getTotalRowCount() == 0)
	{
		LOG("dbGetLoginCharPos() row count : 0", FATAL);
		return false;
	}

	for (RecordSet::Iterator itr = rs->begin(); itr != rs->end(); itr++)
	{
		SharedPtr<CharLoginPos> loginPos = new CharLoginPos;
		loginPos->ID = itr->get(COLIDX(LOGINPOSCOL::ID));
		loginPos->X = itr->get(COLIDX(LOGINPOSCOL::X));
		loginPos->Y = itr->get(COLIDX(LOGINPOSCOL::Y));
		loginPos->Z = itr->get(COLIDX(LOGINPOSCOL::Z));
		loginPos->AxisX = itr->get(COLIDX(LOGINPOSCOL::AXISX));
		loginPos->AxisY = itr->get(COLIDX(LOGINPOSCOL::AXISY));

		charLoginPosTableVec.push_back(loginPos);
	}

	LOG("BaseChar LoginPos Data Table Loaded!", WARNING);

	return true;
}

SharedPtr<CharDataInfo> AlefServerDataSys::getCharDataFromDBID(UInt32 dbID)
{
	for (charDataVec::iterator itr = charDataTableVec.begin(); itr != charDataTableVec.end(); itr++)
	{
		if (itr->get()->ID == dbID)
			return (*itr);
		else
			continue;
	}

	return nullptr;
}

SharedPtr<CharDataInfo> AlefServerDataSys::getCharDataFromTemplID(UInt32 templID)
{
	for (charDataVec::iterator itr = charDataTableVec.begin(); itr != charDataTableVec.end(); itr++)
	{
		if (itr->get()->tID == templID)
			return (*itr);
		else
			continue;
	}

	return nullptr;
}

SharedPtr<TemplateData> AlefServerDataSys::getTemplateFromTID(UInt32 TID)
{
	for (charTemplateVec::iterator itr = templateDataVec.begin(); itr != templateDataVec.end(); itr++)
	{
		if (itr->get()->templateID == TID)
			return (*itr);
		else
			continue;
	}

	return nullptr;
}

Var AlefServerDataSys::getTemplateField(UInt32 TID, UInt32 fieldID)
{
	for (charTemplateVec::iterator itr = templateDataVec.begin(); itr != templateDataVec.end(); itr++)
	{
		if (itr->get()->templateID == TID)
		{
			for (charTemplateFieldVec::iterator fieldItr = itr->get()->templateFields.begin(); fieldItr != itr->get()->templateFields.end(); fieldItr++)
			{
				if (fieldItr->get()->fieldID == fieldID)
					return fieldItr->get()->fieldValue;
				else
					continue;
			}
		}
		else
			continue;
	}

	return nullptr;
}

CharLoginPos AlefServerDataSys::getBaseCharLoginPos(UInt32 baseCharID)
{
	CharLoginPos loginPos;
	loginPos.ID = -1;
	for (charLoginPosVec::iterator itr = charLoginPosTableVec.begin(); itr != charLoginPosTableVec.end(); itr++)
	{
		if (itr->get()->ID == baseCharID)
			return *itr->get();
		else
			continue;
	}

	return loginPos;
}
