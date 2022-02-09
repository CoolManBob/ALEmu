#pragma once

#include "AlefMap.h"

class AlefWorld
{
public:
	AlefWorld();
	~AlefWorld();

	//Load regiontemplate info, create maps with those IDs
	void Init();
	void Update();

private:
	vector<SharedPtr<AlefMap>> maps; 
};