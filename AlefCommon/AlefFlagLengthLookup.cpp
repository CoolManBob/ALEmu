#include "AlefFlagLengthLookup.h"

void AlefFlagLengthLookup::setupTable()
{
	lookupTable[Alef::ALEF_SYSTEM_PACKET_TYPE] = 1;
	lookupTable[Alef::AGSMCHARMANAGER_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMLOGIN_PACKET_TYPE] = 2;
	lookupTable[Alef::AGPMWORLD_PACKET_TYPE] = 1;
	lookupTable[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE] = 1;


	lookupTable[Alef::AGPMLOGIN_CHAR_INFO] = 2;
		/*ALEF_SYSTEM_PACKET_TYPE = 0,
	AGSMCHARMANAGER_PACKET_TYPE = 3,
	AGPMLOGIN_PACKET_TYPE = 0x0D,
	AGPMWORLD_PACKET_TYPE = 0x18,
	AGPMSTARTUPENCRYPTION_PACKET_TYPE = 0x48,*/
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