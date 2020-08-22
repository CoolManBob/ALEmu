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
	std::stringstream res("Success: Config Init");

	//Networking
	loginPort = getInt("AlefLogin.LoginServerPort");

	//Crypto
	cryptoKey = getString("AlefLogin.PacketCryptKey");
	if (cryptoKey.size() != 32)
	{
		res.clear();
		res << endl << "Wrong PacketCryptKey Size Got:" << cryptoKey.size() << " Expected: 32";
	}

	autogenCryptKey = getBool("AlefLogin.UseAutoGenCryptKey");

	//Deprecated
	worldAddress = getString("AlefLogin.WorldAddress");

	//LoginDB
	loginDBAddress = getString("AlefLogin.LoginDBAddress");
	loginDBPort = getString("AlefLogin.LoginDBPort");
	loginDBUser = getString("AlefLogin.LoginDBUser");
	loginDBPass = getString("AlefLogin.LoginDBPass");
	loginDB = getString("AlefLogin.LoginDB");

	//WorldDB
	worldDBAddress = getString("AlefLogin.WorldDBAddress");
	worldDBPort = getString("AlefLogin.WorldDBPort");
	worldDBUser = getString("AlefLogin.WorldDBUser");
	worldDBPass = getString("AlefLogin.WorldDBPass");
	worldDB = getString("AlefLogin.WorldDB");

	//DataDB
	dataDBAddress = getString("AlefLogin.DataDBAddress");
	dataDBPort = getString("AlefLogin.DataDBPort");
	dataDBUser = getString("AlefLogin.DataDBUser");
	dataDBPass = getString("AlefLogin.DataDBPass");
	dataDB = getString("AlefLogin.DataDB");

	return res.str();
}