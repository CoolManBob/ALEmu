#pragma once

#include "AlefDBMapSys.h"
#include "AlefWorld.h"

class AlefServerMapSys
{
public:
	AlefServerMapSys();
	~AlefServerMapSys();

private:
	AlefWorld* world;
};