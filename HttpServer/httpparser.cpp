#include "httpparser.h"

const std::string strtable[3] = { "function","URI","httpVersion" };

TableType HttpParser::operator()(char* requestion)
{
    TableType table;
    char *beg = requestion, *ptr = beg;
    int cnt = 0;
    while (cnt < 3 && *ptr) {
        if (*ptr == ' ' || *ptr == '\r') {
            if (*ptr == '\r')
                *(ptr++) = 0;
            *(ptr++) = 0;
            table.emplace(strtable[cnt++],
                std::move(std::unordered_set<std::string> {
                    std::string(beg) }));
            beg = ptr;
        } else
            ++ptr;
    }
    std::string attrkey;
    while (*ptr) {
        if (*ptr == ':') {
            *(ptr++) = 0;
            attrkey = std::string(beg);
            table.emplace(attrkey,
                std::move(std::unordered_set<std::string>()));
            beg = ptr;
        } else if (*ptr == ',' || *ptr == '\r') {
            if (*ptr == '\r')
                *(ptr++) = 0;
            *(ptr++) = 0;
            table[attrkey].insert(std::move(std::string(beg)));
            beg = ptr;
        } else
            ++ptr;
    };
    return std::move(table);
}