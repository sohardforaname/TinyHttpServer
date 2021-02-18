#pragma once

#include <cstdio>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <exception>

class ConfigurationParser {

private:
    char* Trim(char* buffer);

public:
    std::unordered_map<std::string, std::string> operator()(
        char* buffer,
        size_t len);
};