#pragma once

#include "AlefLoginGlobal.h"
#include "AlefLoginTypes.h"
#include "AlefDBWorldListSys.h"

class AlefServerWorldListSys
{
public:
	AlefServerWorldListSys();
	~AlefServerWorldListSys();

	bool initWorldList();

	string buildWorldInfoStr();

	string getWorldAddress(string worldName);

	bool updateWorldList();

	enum SERVERSTATUS
	{
		CLOSED = 0,
		LOWPOP,
		MEDPOP,
		HIGHPOP,
		FULLPOP,
		EXCESS,
	};

private:
	AlefDBWorldListSys* dbWorldList;
	worldListGroupVec worldList;

	bool getGroupInfo();
	bool getWorldInfo();
};