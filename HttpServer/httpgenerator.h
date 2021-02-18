#pragma once
#include <ctime>
#include <string>
class HttpGenerator
{
public:
    std::string operator()(
        const std::string& contentType,
        const std::string& contentSize);
};