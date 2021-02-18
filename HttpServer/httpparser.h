#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
typedef std::unordered_map<std::string,
    std::unordered_set<std::string>>
    TableType;

class HttpParser
{
public:
    TableType operator()(char* requestion);
};