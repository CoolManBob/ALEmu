#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::stringstream;

#include "AlefWorldGlobal.h"
#include "AlefWorldAGSMChar.h"

bool AlefWorldAGSMChar::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	switch (packet->GetPacketFlag(FlagIndex::FLAG_IDX0))
	{
	case 0x49:
		return processGameEnterCharacterName(sock, packet); break;
	case 0x11:
		return processEnterWorld(sock, packet); break;
	default:
		return false;
	}
	return false;
}

bool AlefWorldAGSMChar::processGameEnterCharacterName(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processGameEnterCharacterName");

	AlefPacket res1(0x01, 0x07, 0x00); //Config Packet
	res1.WriteUInt32(0);
	res1.WriteUInt16(0);
	res1.WriteUInt32(1);
	res1.WriteUInt8(0);
	res1.ClosePacket();

	sock.sendPacket(&res1);

	AlefPacket res2(0x02, 0x03, 0x00); //Event Effect ID
	res2.WriteUInt16(0x40); //Flag
	res2.WriteUInt8(0x19); //Hardcoded Value
	res2.WriteUInt32(1012); //CID
	res2.WriteUInt32(0x4C); //Event Effect ID
	res2.ClosePacket();

	sock.sendPacket(&res2);

	AlefPacket res3(0x03, 0x31, 0x08); //Loading Position
	res3.WriteUInt32(1); //ID
	res3.WriteFloat(-466672); //X
	res3.WriteFloat(3190.88); //Y
	res3.WriteFloat(-46247.6); //Z
	res3.ClosePacket();

	sock.sendPacket(&res3);

	AlefPacket res4(0x11, 0x03, 0x06);
	return true;
}

bool AlefWorldAGSMChar::processEnterWorld(AlefSocket& sock, AlefPacket* packet)
{
	LOG("processEnterWorld");

	sendDummyCharacter(sock);

	AlefPacket res(0x12, 0x07, 0x00);
	res.WriteUInt8(0);
	res.WriteUInt32(1012); //ID

	res.WriteUInt16(0x10); //Size
	res.WriteUInt8(0x0F);
	res.WriteUInt8(1);
	res.WriteUInt32(1012);
	res.WriteByteArray("Dummy#test");
	res.ClosePacket();

	sock.sendPacket(&res);

	return true;
}

void AlefWorldAGSMChar::sendDummyCharacter(AlefSocket& sock)
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

		for (int i = 0; i < 0x20; i++)
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