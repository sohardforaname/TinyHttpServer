#pragma once
#include "Configuration.h"
#include "HandleRequest.h"
#include "pool.hpp"
#include "Log.h"
#include <Windows.h>
#include <csignal>
#include <atomic>

class Server {
private:
    SOCKET serverSocket;
    std::string root;
    size_t maxThread;

private:
    void CreateListenSocket();
    SOCKET Accept();
    static void Handle(SOCKET clientSocket, const std::string& root);
    Server();

public:
    Server(const Server& server) = delete;
    Server& operator=(const Server& server) = delete;
    ~Server();

    static Server& GetInstance();
    void SetMaxThread(size_t maxThread);
    void Loop();
};