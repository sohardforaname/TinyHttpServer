#include "httpgenerator.h"

const std::string head = "HTTP/1.1 200 OK";
const std::string contentType = "Content-Type:";
const std::string date = "Date:";
const std::string host = "Server:";
const std::string contentLength = "Content-Length:";
const std::string newLine = "\r\n";

/*
* 生成响应头字符串
* @param 响应类型字符串，响应体长度字符串
* @return 响应头字符串
*/
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