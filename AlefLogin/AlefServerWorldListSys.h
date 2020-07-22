#pragma once

#include "AlefDBWorldListSys.h"

class AlefServerWorldListSys
{
public:
	AlefServerWorldListSys();
	~AlefServerWorldListSys();

	//std::string buildWorldInfo();

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
};