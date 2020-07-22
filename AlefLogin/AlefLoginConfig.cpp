#include "AlefLoginConfig.h"

AlefLoginConfig::AlefLoginConfig()
{

}

AlefLoginConfig::AlefLoginConfig(std::string configPath)
{
	iniFile = new IniFileConfiguration(configPath);
}

AlefLoginConfig::~AlefLoginConfig()
{

}

std::string AlefLoginConfig::loadConfig()
{
	std::stringstream res("Config Load OK");
	cryptoKey = getString("AlefLogin.PacketCryptKey");
	if (cryptoKey.size() != 32)
		res << "Wrong PacketCryptKey Size Got:" << cryptoKey.size() << " Expected: 32";

	autogenCryptKey = getBool("AlefLogin.UseAutoGenCryptKey");

	worldAddress = getString("AlefLogin.WorldAddress");

	//LoginDB
	loginDBAddress = getString("AlefLogin.LoginDBAddress");
	loginDBUser = getString("AlefLogin.LoginDBUser");
	loginDBPass = getString("AlefLogin.LoginDBPass");
	loginDB = getString("AlefLogin.LoginDB");

	//WorldDB
	worldDBAddress = getString("AlefLogin.WorldDBAddress");
	worldDBUser = getString("AlefLogin.WorldDBUser");
	worldDBPass = getString("AlefLogin.WorldDBPass");
	worldDB = getString("AlefLogin.WorldDB");

	//DataDB
	dataDBAddress = getString("AlefLogin.DataDBAddress");
	dataDBUser = getString("AlefLogin.DataDBUser");
	dataDBPass = getString("AlefLogin.DataDBPass");
	dataDB = getString("AlefLogin.DataDB");

	return res.str();
}