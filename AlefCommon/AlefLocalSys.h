#pragma once

#include "AlefLocalTypes.h"

class AlefLocalSys
{
public:
	AlefLocalSys();
	~AlefLocalSys();

	AlefClientAccount* _localAcct() { return localAcct; }
	AlefClientCharacter* _localChar() { return localChar; }

private:
	AlefClientAccount* localAcct;
	AlefClientCharacter* localChar;
};