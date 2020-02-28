#pragma once

#include <Poco/Foundation.h>
#include <Poco/HashMap.h>

using Poco::UInt8;
using Poco::UInt16;

using Poco::HashMap;

#include "AlefTypes.h"

class AlefFlagLengthLookup
{
public:
	AlefFlagLengthLookup() { setupTable(); };
	~AlefFlagLengthLookup() {};

	UInt8 lookUp(UInt16 packetType);

private:
	void setupTable();

	HashMap<UInt16, UInt8> lookupTable;

};