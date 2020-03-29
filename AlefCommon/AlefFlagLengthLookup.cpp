#include "AlefFlagLengthLookup.h"

void AlefFlagLengthLookup::setupTable()
{
	//Main Packets
	lookupTable[Alef::ALEF_SYSTEM_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMCONFIG_PACKET_TYPE] = 4;
	lookupTable[Alef::AGPMCHARACTER_PACKET_TYPE] = 4;
	lookupTable[Alef::AGSMCHARMANAGER_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMITEM_PACKET_TYPE] = 4;
	lookupTable[Alef::AGPMLOGIN_PACKET_TYPE] = 2;
	lookupTable[Alef::AGPMTIMER_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMUISTATUS_PACKET_TYPE] = 2;
	lookupTable[Alef::AGPMWORLD_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMOPTIMIZEDCHARACTION_PACKET_TYPE] = 2;
	lookupTable[Alef::AGPMOPTIMIZEDVIEW_PACKET_TYPE] = 2;
	lookupTable[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE] = 1;

	//Mini Packets
	lookupTable[Alef::AGPMLOGIN_SERVER_ADDR] = 1;
	lookupTable[Alef::AGPMLOGIN_CHAR_INFO] = 2;
	lookupTable[Alef::AGPMLOGIN_SERVER_INFO] = 1;
	lookupTable[Alef::AGPMLOGIN_VERSION_INFO] = 1;
	lookupTable[Alef::AGPMLOGIN_COMPEN_INFO] = 1;
	
	lookupTable[Alef::AGPMCHAR_MOVE] = 1;
	lookupTable[Alef::AGPMCHAR_FACTORS] = 2;
	lookupTable[Alef::AGPMCHAR_FACTOR_STATUS] = 2;
	lookupTable[Alef::AGPMCHAR_FACTOR_TYPE] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_POINT] = 4;
	lookupTable[Alef::AGPMCHAR_FACTOR_POINTMAX] = 2;
	lookupTable[Alef::AGPMCHAR_FACTOR_RECOVERY] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_ATTRIBUTE] = 2;
	lookupTable[Alef::AGPMCHAR_FACTOR_DMG] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_DEFENSE] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_ATTACK] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_ITEM] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_DIRT] = 1;
	lookupTable[Alef::AGPMCHAR_FACTOR_PRICE] = 1;

	lookupTable[Alef::AGPMOPTIMIZEDPACKET_VIEW] = 4;
	lookupTable[Alef::AGPMOPTIMIZEDPACKET_ITEMVIEW] = 4;

}

UInt8 AlefFlagLengthLookup::lookUp(UInt16 packetType)
{
	HashMap<UInt16, UInt8>::Iterator Itr = lookupTable.find(packetType);
	if (Itr != lookupTable.end())
	{
		return Itr->second;
	}
	else
		return 0xFF;
}