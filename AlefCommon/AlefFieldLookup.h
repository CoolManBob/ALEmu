#pragma once

#include <Poco/HashMap.h>

using Poco::HashMap;

#include <vector>
using std::vector;

#include "AlefTypes.h"

class AlefFieldLookup
{
public:
	AlefFieldLookup();
	~AlefFieldLookup();

	bool getFieldInfo(vector<Alef::AlefPktField>& fieldVec, UInt16 packetType);

private:
	void initMainMaps();
	void initMiniMaps();

	vector<UInt8> buildTypeVec(UInt32 sz, UInt8 types[]);
	vector<UInt8> buildTypeVec(vector<UInt8> types);
	vector<UInt32> buildSizeVec(UInt32 sz, UInt32 sizes[]);
	vector<UInt32> buildSizeVec(vector<UInt32> sizes);
	
	HashMap<UInt16, vector<UInt8>> typeMap;
	HashMap<UInt16, vector<UInt32>> sizeMap;
};