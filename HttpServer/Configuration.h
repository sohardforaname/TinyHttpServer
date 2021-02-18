#pragma once
#include <cstdio>
#include "ConfigurationParser.h"
#include <memory>

class Configuration
{
private:
    std::unordered_map<std::string, std::string> table;

public:
	void ReadConfigurationFile(const char* filePath);
	std::string GetValue(const std::string& key);
	Configuration();
	~Configuration();
};