#include "AlefLocalSys.h"

AlefLocalSys::AlefLocalSys()
{
	localAcct = new AlefClientAccount;
	localChar = new AlefClientCharacter;
}

AlefLocalSys::~AlefLocalSys()
{
	delete localAcct;
	delete localChar;
}