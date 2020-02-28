#include "AlefFieldLookup.h"

AlefFieldLookup::AlefFieldLookup()
{
	initMainMaps();
	//initMiniMaps();
}

AlefFieldLookup::~AlefFieldLookup()
{

}

void AlefFieldLookup::initMainMaps()
{
	//typeMap[0] = buildTypeVec();
	//typeMap[3] = 1;
	//typeMap[0x0D] = 2;
	//typeMap[0x18] = 2;
	//typeMap[0x48] = buildTypeVec({ 1, 13 });
	typeMap[Alef::AGPMLOGIN_PACKET_TYPE] = buildTypeVec({Alef::INT8, Alef::CHAR, Alef::CHAR, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT32, Alef::CHAR,
														Alef::PACKET, Alef::PACKET, Alef::INT32, Alef::PACKET, Alef::CHAR, Alef::CHAR, Alef::INT32, Alef::INT32});
	typeMap[Alef::AGPMWORLD_PACKET_TYPE] = buildTypeVec({ Alef::INT8, Alef::CHAR, Alef::INT16, Alef::MEMORY_BLOCK });
	typeMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE] = buildTypeVec({ Alef::INT8, Alef::MEMORY_BLOCK });
	//typeMap[0x48] = buildTypeVec({ 1, 13 });
	

	//sizeMap[0] = 1;
	//sizeMap[3] = 1;
	//sizeMap[0x0D] = 2;
	//sizeMap[0x18] = 2;
	sizeMap[Alef::AGPMLOGIN_PACKET_TYPE] = buildSizeVec({ 1, 32, 49, 1, 33, 1, 1, 32, 1, 1, 1, 1, 2049, 5, 1, 1});
	sizeMap[Alef::AGPMWORLD_PACKET_TYPE] = buildSizeVec({ 1, 32, 1, 1 });
	sizeMap[Alef::AGPMSTARTUPENCRYPTION_PACKET_TYPE] = buildSizeVec({ 1, 1 });
	//sizeMap[0x48] = buildSizeVec({ 1, 1 });
}

void AlefFieldLookup::initMiniMaps()
{
	//typeMap[Alef::AGPMCHAR_FACTOR_RES] = buildTypeVec({ 1, 1 });
	//sizeMap[Alef::AGPMCHAR_FACTOR_RES] = buildSizeVec({ 1, 1 });
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