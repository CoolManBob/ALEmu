#include "AlefClientCharacter.h"

AlefClientCharacter::AlefClientCharacter()
{
	acctID = 0;
}

AlefClientCharacter::~AlefClientCharacter()
{

}

CharacterData AlefClientCharacter::getCharAtSlot(Int32 slotIdx)
{
	CharacterData nullData;
	nullData.charID = -1;
	for (clientCharDataVec::iterator itr = characters.begin(); itr != characters.end(); itr++)
	{
		UInt32 slot = itr->get()->slot;
		if (slot == slotIdx)
			return *itr->get();
	}

	return nullData;
}

UInt32 AlefClientCharacter::getAvailableSlotNum()
{
	UInt32 availSlot = 0;
	if (characters.size() == 0)
		return availSlot;
	else
	{
		for (clientCharDataVec::iterator itr = characters.begin(); itr != characters.end(); itr++)
		{
			UInt32 slot = itr->get()->slot;
			if (slot > availSlot)
				availSlot = slot;
		}
		return availSlot + 1;
	}

	return -1;
}

SharedPtr<AlefPacket> AlefClientCharacter::buildCharPkt(CharacterData& data)
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

SharedPtr<AlefPacket> AlefClientCharacter::buildCharMovePacket(CharacterMove& move)
{
	SharedPtr<AlefPacket> charMovePkt;

	//{	Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::INT32, Alef::FLOAT, Alef::FLOAT, Alef::INT8, Alef::INT8 }
	charMovePkt = pktInterface->buildMiniPacket(Alef::AGPMCHAR_MOVE, &move.curPos, &move.destPos, &move.followTargetId, &move.followDistance, &move.axisX, &move.axisY, &move.flags, &move.direction);

	return charMovePkt;
}

SharedPtr<AlefPacket> AlefClientCharacter::buildFactorPacket(CharacterFactors& factors)
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

SharedPtr<AlefPacket> AlefClientCharacter::buildFactorResultPacket(CharacterFactorResult& result)
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