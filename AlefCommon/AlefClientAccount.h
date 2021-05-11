#pragma once

#include <string>
using std::string;

#include "AlefTypes.h"

struct AlefAcctData
{
	UInt32 acctID;
	std::string acctUser;
	std::string acctPw;
	UInt32 acctStatus;
};

class AlefClientAccount
{
	friend class AlefLocalSys;
public:
	AlefClientAccount();
	~AlefClientAccount();

	bool decryptUserInfo(string& userName, Int32 acctLen, string& password, Int32 pwLen);
	string generateHashKey();

	Int32 getLoginStep() { return loginStep; }
	void setLoginStep(Int32 step) { loginStep = step; }

	string getUserAcct() { return data.acctUser; }
	UInt32 getUserID() { return data.acctID; }


protected:
	AlefAcctData& getLocalData() { return data; }
	void setHashKey(string key) { hashKey = key; }
	string getHashKey() { return hashKey; }

private:
	bool decrypt(UInt8* str, DWORD len);

	AlefAcctData data;
	Int32 loginStep;
	string hashKey;
};