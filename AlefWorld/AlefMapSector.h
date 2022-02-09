#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

#include <Poco/SharedPtr.h>
using Poco::SharedPtr;

#include "AlefTypes.h"

class AlefMapSector
{
public:
	AlefMapSector();
	~AlefMapSector();

	//
	void Init();
	void Update();

private:
	float startX;
	float startZ;

	float endX;
	float endZ;

	float idxX;
	float idxZ;

	string sectorName;

	vector<SharedPtr<AlefPos>> playerPos;
	vector<SharedPtr<AlefPos>> npcPos;
};