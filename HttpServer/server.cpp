#include "server.h"

static std::atomic_bool runFlag(true);

/*
* 创建监听socket
* @param 
* @return 
*/
void Server::CreateListenSocket()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("wsa init failed\n");
        exit(-1);
    }
    Configuration configuration;
    configuration.ReadConfigurationFile("/testhtml/serverconfig");

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    unsigned long ul = 1;
    ioctlsocket(serverSocket, FIONBIO, &ul);

    root = configuration.GetValue("root");
    printf(
        "Server run at: %s:%d\n\n",
        configuration.GetValue("ip").c_str(),
        atoi(configuration.GetValue("port").c_str()));

    SOCKADDR_IN sckaddrin;
    memset(&sckaddrin, 0, sizeof(sckaddrin));
    sckaddrin.sin_family = AF_INET;
    sckaddrin.sin_port = htons(atoi(configuration.GetValue("port").c_str()));
    sckaddrin.sin_addr.s_addr = inet_addr(configuration.GetValue("ip").c_str());

    if (bind(serverSocket, (sockaddr*)(&sckaddrin), sizeof(sckaddrin)) < 0) {
        printf("bind failed\n");
        closesocket(serverSocket);
        exit(-1);
    }

    if (listen(serverSocket, 10) < 0) {
        printf("listen error\n");
        closesocket(serverSocket);
        exit(-1);
    }
}

/*
* 非阻塞地等待客户端的请求
* @param
* @return 客户端请求的socket id， 失败返回-1
*/
SOCKET Server::Accept()
{
    SOCKADDR_IN clsckaddrin;
    int nameLen = sizeof(clsckaddrin);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)(&clsckaddrin), &nameLen);
    if (clientSocket == SOCKET_ERROR || WSAGetLastError()) {
        //Sleep(100);
        return -1;
    }
    printf("client address: %s:%d\n",
        inet_ntoa(clsckaddrin.sin_addr), ntohs(clsckaddrin.sin_port));
    return clientSocket;
}

/*
* 处理Accept函数中获取的客户端请求socket
* @param 客户端请求socket；服务器文件根目录
* @return
*/
void Server::Handle(SOCKET clientSocket, const std::string& root)
{
    auto buffer = std::unique_ptr<char[]>(new char[1 << 10]);
    int receiveLength = -1;
    // memset(buffer.get(), 0, 1 << 10);
    while (receiveLength == -1) {
        receiveLength = recv(clientSocket, buffer.get(), 1 << 10, 0);
        if (!runFlag.load())
            return;
        if (receiveLength == -1) {
            if (WSAEWOULDBLOCK == WSAGetLastError()) {
                //Sleep(100);
                continue;
            } else {
                return;
            }
        }
    }
    if (receiveLength < 0) {
        // printf("recv error\n");
        return;
    }
    buffer[receiveLength] = 0;
    //printf("%s\n", buffer.get());
    auto request = std::unique_ptr<HandleRequest>(new HandleRequest(
        buffer.get(),
        clientSocket,
        root));
    request->SendResponse();
}

/*
* 处理退出信号
* @param 系统的信号
* @return
*/
void HandleSignal(int sign)
{
    runFlag.store(false);
    //system("pause");
}

/*
* 服务器主函数
* @param
* @return
*/
void Server::Loop()
{
    ThreadPool pool(maxThread);
    while (runFlag.load()) {
        SOCKET acceptedSocket = Accept();
        if (acceptedSocket == -1)
            continue;
        pool.CommitNormalFunction(
            Handle,
            acceptedSocket,
            this->root);
        // Handle(acceptedSocket, this->root);
    }
};

Server& Server::GetInstance() 
{
    static Server server;
    return server;
}

void Server::SetMaxThread(size_t maxThread)
{
    this->maxThread = maxThread ? maxThread : 1;
}

Server::Server()
    : maxThread(1)
{
    signal(SIGINT, HandleSignal);
    CreateListenSocket();
}

Server::~Server()
{
	printf("Shutdown server\n");
	closesocket(serverSocket);
	WSACleanup();
}