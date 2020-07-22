#pragma once

#include "AlefConfig.h"
#include <sstream>

class AlefLoginConfig : AlefConfig
{
public:
	AlefLoginConfig();
	AlefLoginConfig(std::string configPath);
	~AlefLoginConfig();

	std::string loadConfig();

	std::string getCryptKey() { return cryptoKey; }
	bool		getAutoGenKey() { return autogenCryptKey;  }

	std::string getWorldAddress() { return worldAddress; } //TODO: Move to DB
	
	//LoginDB
	std::string getLoginDBAddress() { return loginDBAddress; }
	std::string getLoginDBUser() { return loginDBUser; }
	std::string getLoginDBPass() { return loginDBPass; }
	std::string getLoginDB() { return loginDB; }
	
	//WorldDB
	std::string getWorldDBAddress() { return worldDBAddress; }
	std::string getWorldDBUser() { return worldDBUser; }
	std::string getWorldDBPass() { return worldDBPass; }
	std::string getWorldDB() { return worldDB; }
	
	//DataDB
	std::string getDataDBAddress() { return dataDBAddress; }
	std::string getDataDBUser() { return dataDBUser; }
	std::string getDataDBPass() { return dataDBPass; }
	std::string getDataDB() { return dataDB; }
	

private:
#pragma region Configuration Values
	
	//Encryption
	std::string cryptoKey;
	bool		autogenCryptKey;

	//ServerList
	std::string worldAddress;

	//LoginDB
	std::string loginDBAddress;
	std::string loginDBUser;
	std::string loginDBPass;
	std::string loginDB;

	//WorldDB
	std::string worldDBAddress; //currently databases cannot be segmented to different addresses
	std::string worldDBUser;
	std::string worldDBPass;
	std::string worldDB;

	//DataDB
	std::string dataDBAddress; //currently databases cannot be segmented to different addresses
	std::string dataDBUser;
	std::string dataDBPass;
	std::string dataDB;

#pragma endregion
};