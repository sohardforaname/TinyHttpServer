#include "ConfigurationParser.h"

char* ConfigurationParser::Trim(char* buffer)
{
    char* ptr = buffer;
    while ((*ptr) && !isgraph(*ptr)) {
        ++ptr;
    }
    char* res = ptr;
    while ((*ptr) && isgraph(*ptr) && (*ptr) != '=') {
        ++ptr;
    }
    *ptr = 0;
    return res;
}

std::unordered_map<std::string, std::string> ConfigurationParser::operator()(
    char* buffer,
    size_t len)
{
    int st = 0, ed = len;
    std::unordered_map<std::string, std::string> table;
    std::string key, val;
    while (st < len) {
        size_t pos = std::find(buffer + st, buffer + ed, '\n') - buffer;
        size_t equ = std::find(buffer + st, buffer + pos, '=') - buffer;
        table.emplace(
            std::move(std::string(Trim(buffer + st))),
            std::move(std::string(Trim(buffer + equ + 1))));
        st = pos + 1;
    }
    return std::move(table);
}