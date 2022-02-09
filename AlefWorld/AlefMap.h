#pragma once

#include "AlefMapSector.h"

class AlefMap
{
public:
	AlefMap();
	~AlefMap();

	//initializes sector data for it's own specific region
	void Init(int regionID);
	void Update();

private:
	int mapID;

	vector<SharedPtr<AlefMapSector>> mapSectors;
};