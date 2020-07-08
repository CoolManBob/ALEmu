#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Util/IniFileConfiguration.h>

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

class AlefConfig
{
public:
	AlefConfig() {}
	AlefConfig(std::string fileName) { iniFile = new IniFileConfiguration(fileName); }
	virtual ~AlefConfig() {}

	bool getBool(std::string key) { return iniFile->getBool(key, false); }
	double getDouble(std::string key) { return iniFile->getDouble(key, 0); }
	int getInt(std::string key) { return iniFile->getInt(key, 0); }
	std::string getString(std::string key) { return iniFile->getString(key, ""); }

protected:
	AutoPtr<IniFileConfiguration> iniFile;
};