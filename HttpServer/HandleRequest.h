#pragma once
#include "HttpParser.h"
#include <WinSock2.h>
#include "Reader.h"
#include "httpgenerator.h"
#include <memory>
#pragma comment (lib, "ws2_32")

class HandleRequest {
private:
    SOCKET clientSocket;
    TableType table;
    std::shared_ptr<std::string> rootPath;

private:
    HandleRequest();
    const std::string& GetTypeFromFilePath(const char* filePath);
    const std::string& GetFirstValue(const std::string& key);

public:
    HandleRequest(
        char* buffer, 
        const SOCKET& socket,
        const std::string& root);
    void SendResponse();
    ~HandleRequest();
};