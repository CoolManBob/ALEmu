#include "AlefFieldLookup.h"

AlefFieldLookup::AlefFieldLookup()
{
	initMainMaps();
	initMiniMaps();
}

AlefFieldLookup::~AlefFieldLookup()
{

}

void AlefFieldLookup::initMainMaps()
{
#pragma region TypeMaps

	typeMap[Alef::AGPMCONFIG_PACKET_TYPE]				=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT8 });


	typeMap[Alef::AGPMCHARACTER_PACKET_TYPE]			=	buildTypeVec({	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET, 
																			Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8, 
																			Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32, 
																			Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32 });

	typeMap[Alef::AGSMCHARMANAGER_PACKET_TYPE]			=	buildTypeVec({	Alef::INT8, Alef::CHAR, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::VEC3F, Alef::INT32 });


	typeMap[Alef::AGPMITEM_PACKET_TYPE]					=	buildTypeVec({	Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::PACKET, Alef::PACKET,
																			Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::PACKET, Alef::PACKET,
																			Alef::PACKET, Alef::INT32, Alef::UINT32, Alef::INT32, Alef::PACKET, Alef::PACKET, Alef::UINT32, Alef::PACKET, Alef::PACKET });

	typeMap[Alef::AGPMTIMER_PACKET_TYPE]				=	buildTypeVec({	Alef::UINT8, Alef::UINT64 });

	typeMap[Alef::AGPMLOGIN_PACKET_TYPE]				=	buildTypeVec({	Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR,
																			Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32	});

	typeMap[Alef::AGPMUISTATUS_PACKET_TYPE]				=	buildTypeVec({	Alef::INT8, Alef::INT32, Alef::PACKET, Alef::INT8, Alef::PACKET, Alef::MEMORY_BLOCK, Alef::INT32, Alef::INT32, Alef::INT8, 
																			Alef::INT8, Alef::INT8, Alef::MEMORY_BLOCK });
	
	typeMap[Alef::AGPMWORLD_PACKET_TYPE]				=	buildTypeVec({	Alef::INT8, Alef::CHAR, Alef::INT16, Alef::MEMORY_BLOCK, Alef::CHAR });

	typeMap[Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE]	=	buildTypeVec({	Alef::INT32, Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::UINT16, Alef::INT8, Alef::INT8, Alef::INT8, Alef::INT32 });

	typeMap[Alef::AGPMOPTIMIZEDCHARACTION_PACKET_TYPE]	=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT8, Alef::PACKET, Alef::INT32, Alef::INT8, Alef::INT8, Alef::UINT32, Alef::UINT8 });

	typeMap[Alef::AGPMOPTIMIZEDVIEW_PACKET_TYPE]		=	buildTypeVec({	Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::MEMORY_BLOCK, Alef::MEMORY_BLOCK, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::MEMORY_BLOCK,
																			Alef::MEMORY_BLOCK, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK });
	
	typeMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE]	=	buildTypeVec({	Alef::INT8, Alef::MEMORY_BLOCK });

#pragma endregion

#pragma region SizeMaps

	sizeMap[Alef::AGPMCONFIG_PACKET_TYPE]				=	buildSizeVec({	1, 1, 1 });

	sizeMap[Alef::AGPMCHARACTER_PACKET_TYPE]			=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 32, 1, 1, 1, 1, 1, 1, 1, 16, 1, 1, });

	sizeMap[Alef::AGSMCHARMANAGER_PACKET_TYPE]			=	buildSizeVec({	1, 12, 1, 49, 1, 1, 1 });
	
	sizeMap[Alef::AGPMITEM_PACKET_TYPE]					=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMTIMER_PACKET_TYPE]				=	buildSizeVec({	1, 1 });
	
	sizeMap[Alef::AGPMLOGIN_PACKET_TYPE]				=	buildSizeVec({	1, 32, 49, 1, 33, 1, 1, 32, 1, 1, 1, 1, 2049, 5, 1, 1});

	sizeMap[Alef::AGPMUISTATUS_PACKET_TYPE]				=	buildSizeVec({	1, 1, 40, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
	
	sizeMap[Alef::AGPMWORLD_PACKET_TYPE]				=	buildSizeVec({	1, 32, 1, 1, 32 });

	sizeMap[Alef::AGPMOPTIMIZEDCHARMOVE_PACKET_TYPE]	=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMOPTIMIZEDCHARACTION_PACKET_TYPE]	=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMOPTIMIZEDVIEW_PACKET_TYPE]		=	buildSizeVec({	1, 1, 31, 1, 1, 33, 1, 1, 1, 1, 1, 1, 1, 1 });
	
	sizeMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE]	=	buildSizeVec({	1, 1 });

#pragma endregion
}

void AlefFieldLookup::initMiniMaps()
{
#pragma region TypeMaps

#pragma region AGPMLOGIN

	typeMap[Alef::AGPMLOGIN_SERVER_ADDR]		=	buildTypeVec({	Alef::CHAR, Alef::CHAR, Alef::CHAR });

	typeMap[Alef::AGPMLOGIN_CHAR_INFO]			=	buildTypeVec({	Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::CHAR });

	typeMap[Alef::AGPMLOGIN_SERVER_INFO]		=	buildTypeVec({	Alef::CHAR });

	typeMap[Alef::AGPMLOGIN_VERSION_INFO]		=	buildTypeVec({	Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMLOGIN_COMPEN_INFO]		=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::CHAR, Alef::CHAR, Alef::MEMORY_BLOCK });

#pragma endregion
	
#pragma region AGPMCHARACTER

	typeMap[Alef::AGPMCHAR_MOVE]				=	buildTypeVec({	Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::INT32, Alef::FLOAT, Alef::FLOAT, Alef::INT8, Alef::INT8 });

	typeMap[Alef::AGPMCHAR_SKILL]				=	buildTypeVec({ Alef::INT8, Alef::INT32, Alef::INT32, Alef::INT8, Alef::PACKET, Alef::VEC3F, Alef::UINT8, Alef::INT8, Alef::UINT32, Alef::UINT8 });
	
	typeMap[Alef::AGPMCHAR_FACTORS]				=	buildTypeVec({	Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET, Alef::PACKET });
	
	typeMap[Alef::AGPMCHAR_FACTOR_STATUS]		=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });
	
	typeMap[Alef::AGPMCHAR_FACTOR_TYPE]			=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT32 });
	
	typeMap[Alef::AGPMCHAR_FACTOR_POINT]		=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, 
																Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_POINTMAX]		=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_RECOVERY]		=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_ATTRIBUTE]	=	buildTypeVec({ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_DMG]			=	buildTypeVec({ Alef::PACKET, Alef::PACKET });
	
	typeMap[Alef::AGPMCHAR_FACTOR_DEFENSE]		=	buildTypeVec({ Alef::PACKET, Alef::PACKET });

	typeMap[Alef::AGPMCHAR_FACTOR_ATTACK]		=	buildTypeVec({ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_ITEM]			=	buildTypeVec({ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_DIRT]			=	buildTypeVec({ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMCHAR_FACTOR_PRICE]		=	buildTypeVec({ Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32 });

#pragma endregion

#pragma region AGPMOPTIMIZEDPACKET2

	typeMap[Alef::AGPMOPTIMIZEDPACKET_VIEW]				=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::INT8, Alef::PACKET, Alef::INT8, Alef::INT8, Alef::VEC3F,
																			Alef::VEC3F, Alef::INT32, Alef::UINT16, Alef::INT16, Alef::INT16, Alef::UINT64, Alef::INT8, Alef::INT8, Alef::INT8,
																			Alef::INT8, Alef::UINT16, Alef::INT8, Alef::INT32, Alef::CHAR, Alef::INT32, Alef::INT32 });

	typeMap[Alef::AGPMOPTIMIZEDPACKET_ITEMVIEW]			=	buildTypeVec({	Alef::INT32, Alef::INT32, Alef::INT16, Alef::INT8, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32,
																			Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::INT32, Alef::UINT16, Alef::UINT16, Alef::UINT16, Alef::UINT16, Alef::UINT16 });

#pragma endregion

#pragma endregion

#pragma region SizeMaps

#pragma region AGPMLOGIN

	sizeMap[Alef::AGPMLOGIN_SERVER_ADDR]		=	buildSizeVec({ 23, 23, 23 });
	
	sizeMap[Alef::AGPMLOGIN_CHAR_INFO]			=	buildSizeVec({ 1, 49, 1, 1, 1, 1, 1, 1, 49 });

	sizeMap[Alef::AGPMLOGIN_SERVER_INFO]		=	buildSizeVec({ 23 });

	sizeMap[Alef::AGPMLOGIN_VERSION_INFO]		=	buildSizeVec({ 1, 1 });

	sizeMap[Alef::AGPMLOGIN_COMPEN_INFO]		=	buildSizeVec({ 1, 1, 49, 100, 1 });

#pragma endregion

#pragma region AGPMCHARACTER

	sizeMap[Alef::AGPMCHAR_MOVE]				=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1 });
	
	sizeMap[Alef::AGPMCHAR_FACTORS]				=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
	
	sizeMap[Alef::AGPMCHAR_FACTOR_STATUS]		=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
	
	sizeMap[Alef::AGPMCHAR_FACTOR_TYPE]			=	buildSizeVec({ 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_POINT]		=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_POINTMAX]		=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_RECOVERY]		=	buildSizeVec({ 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_ATTRIBUTE]	=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_DMG]			=	buildSizeVec({ 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_DEFENSE]		=	buildSizeVec({ 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_ATTACK]		=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_ITEM]			=	buildSizeVec({ 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_DIRT]			=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_FACTOR_PRICE]		=	buildSizeVec({ 1, 1, 1, 1 });

	sizeMap[Alef::AGPMCHAR_SKILL]				=	buildSizeVec({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

#pragma endregion

#pragma region AGPMOPTIMIZEDPACKET2

	sizeMap[Alef::AGPMOPTIMIZEDPACKET_VIEW]				=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 16, 1, 1 });

	sizeMap[Alef::AGPMOPTIMIZEDPACKET_ITEMVIEW]			=	buildSizeVec({	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, });

#pragma endregion

#pragma endregion
}

vector<UInt8> AlefFieldLookup::buildTypeVec(UInt32 sz, UInt8 types[])
{
	vector<UInt8> vec;

	for (int i = 0; i < sz; i++)
		vec.push_back(types[i]);

	return vec;
}

vector<UInt8> AlefFieldLookup::buildTypeVec(vector<UInt8> types)
{
	vector<UInt8> vec;

	for (int i = 0; i < types.size(); i++)
		vec.push_back(types[i]);

	return vec;
}

vector<UInt32> AlefFieldLookup::buildSizeVec(UInt32 sz, UInt32 sizes[])
{
	vector<UInt32> vec;

	for (int i = 0; i < sz; i++)
		vec.push_back(sizes[i]);

	return vec;
}

vector<UInt32> AlefFieldLookup::buildSizeVec(vector<UInt32> sizes)
{
	vector<UInt32> vec;

	for (int i = 0; i < sizes.size(); i++)
		vec.push_back(sizes[i]);

	return vec;
}

bool AlefFieldLookup::getFieldInfo(vector<Alef::AlefPktField>& fieldVec, UInt16 packetType)
{
	vector<UInt8> fields;
	vector<UInt32> sizes;

	HashMap<UInt16, vector<UInt8>>::Iterator typeItr = typeMap.find(packetType);
	if (typeItr != typeMap.end())
	{
		for (vector<UInt8>::iterator Itr = typeItr->second.begin(); Itr != typeItr->second.end(); Itr++)
			fields.push_back(*Itr);
	}
	else
		return false; //No type info for given packet type

	HashMap<UInt16, vector<UInt32>>::Iterator szItr = sizeMap.find(packetType);
	if (szItr != sizeMap.end())
	{
		for (vector<UInt32>::iterator Itr = szItr->second.begin(); Itr != szItr->second.end(); Itr++)
			sizes.push_back(*Itr);
	}
	else
		return false; //No size info for given packet type

	if (fields.size() != sizes.size()) //Types and Sizes should have the same number of elements
	{
		//ERROR
		return false;
	}
	
	for (int i = 0; i < fields.size(); i++)
	{
		Alef::AlefPktField field;
		field.FieldType = fields[i];
		field.FieldSize = sizes[i];

		fieldVec.push_back(field);
	}

	return true;
}