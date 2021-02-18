#include "httpgenerator.h"

const std::string head = "HTTP/1.1 200 OK";
const std::string contentType = "Content-Type:";
const std::string date = "Date:";
const std::string host = "Server:";
const std::string contentLength = "Content-Length:";
const std::string newLine = "\r\n";

std::string HttpGenerator::operator()(
    const std::string& contentType,
    const std::string& contentSize)
{
    time_t curTime;
    time(&curTime);
    std::string str("");
    str += head + newLine;
    str += (contentType + contentType + newLine);
    str += (contentLength + contentSize + newLine);
    str += (host + "localhost" + newLine);
    str += (date + ctime(&curTime) + newLine);
    return str;
}