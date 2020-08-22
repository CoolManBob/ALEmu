#pragma once

#include "AlefTypes.h"

struct AlefClientData
{
	UInt32 acctID;
	std::string acctPw;
	UInt32 acctStatus;
};


class AlefClientAccount
{
public:
	AlefClientAccount();
	~AlefClientAccount();

private:
	AlefClientData data;
};