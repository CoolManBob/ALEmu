#pragma once

#include "AlefDBLoginSys.h"

class AlefServerLoginSys
{
public:
	AlefServerLoginSys();
	~AlefServerLoginSys();

	bool checkLogin(string username, string password);

private:
	AlefDBLoginSys* dbLoginSys;
};