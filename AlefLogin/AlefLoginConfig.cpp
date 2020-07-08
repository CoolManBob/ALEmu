#include "AlefLoginConfig.h"

AlefLoginConfig::AlefLoginConfig()
{

}

AlefLoginConfig::AlefLoginConfig(std::string configPath)
{
	iniFile = new IniFileConfiguration(configPath);
	loadConfig();
}

AlefLoginConfig::~AlefLoginConfig()
{

}

void AlefLoginConfig::loadConfig()
{
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
}