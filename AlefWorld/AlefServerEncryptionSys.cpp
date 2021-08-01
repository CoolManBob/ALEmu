#include "Poco/RandomStream.h"
#include <iostream>

using Poco::RandomInputStream;

#include "AlefServerEncryptionSys.h"

AlefServerEncryptionSys::AlefServerEncryptionSys()
{

}

AlefServerEncryptionSys::~AlefServerEncryptionSys()
{

}

std::string AlefServerEncryptionSys::getCryptKey()
{
	//KeyGen
	RandomInputStream ri;
	std::string rs;
	ri >> rs;
	rs.resize(32);
	return rs;
}