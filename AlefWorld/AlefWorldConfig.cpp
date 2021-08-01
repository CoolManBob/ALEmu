#include "AlefWorldConfig.h"

AlefWorldConfig::AlefWorldConfig()
{

}

AlefWorldConfig::AlefWorldConfig(std::string configPath)
{
	iniFile = new IniFileConfiguration(configPath);
}

AlefWorldConfig::~AlefWorldConfig()
{

}

std::string AlefWorldConfig::loadConfig()
{
	std::stringstream res("Success: Config Init");

	//Networking
	worldPort = getInt("AlefWorld.WorldServerPort");

	//Crypto
	cryptoKey = getString("AlefWorld.PacketCryptKey");
	if (cryptoKey.size() != 32)
	{
		res.clear();
		res << endl << "Wrong PacketCryptKey Size Got:" << cryptoKey.size() << " Expected: 32";
	}

	autogenCryptKey = getBool("AlefWorld.UseAutoGenCryptKey");

	//LoginDB
	loginDBAddress = getString("AlefWorld.LoginDBAddress");
	loginDBPort = getString("AlefWorld.LoginDBPort");
	loginDBUser = getString("AlefWorld.LoginDBUser");
	loginDBPass = getString("AlefWorld.LoginDBPass");
	loginDB = getString("AlefWorld.LoginDB");

	//WorldDB
	worldDBAddress = getString("AlefWorld.WorldDBAddress");
	worldDBPort = getString("AlefWorld.WorldDBPort");
	worldDBUser = getString("AlefWorld.WorldDBUser");
	worldDBPass = getString("AlefWorld.WorldDBPass");
	worldDB = getString("AlefWorld.WorldDB");

	//DataDB
	dataDBAddress = getString("AlefWorld.DataDBAddress");
	dataDBPort = getString("AlefWorld.DataDBPort");
	dataDBUser = getString("AlefWorld.DataDBUser");
	dataDBPass = getString("AlefWorld.DataDBPass");
	dataDB = getString("AlefWorld.DataDB");

	return res.str();
}