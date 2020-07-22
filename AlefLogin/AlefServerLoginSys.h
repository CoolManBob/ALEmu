#pragma once

#include "AlefDBLoginSys.h"

class AlefServerLoginSys
{
public:
	AlefServerLoginSys();
	~AlefServerLoginSys();

	//int checkLogin

private:
	AlefDBLoginSys* dbLoginSys;
};