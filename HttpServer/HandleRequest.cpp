#include "handlerequest.h"

HandleRequest::HandleRequest(
    char* buffer,
    const SOCKET& socket,
    const std::string& root)
    : clientSocket(socket)
    , table(HttpParser()(buffer))
    , rootPath(std::make_shared<std::string>(root)) {}

const std::unordered_map<std::string, std::string> contentTypeMap = {
    { "html", "text/html" },
    { "xml", "text/xml" },
    { "ico", "image/x-icon" },
    { "jpg", "image/jpeg" },
    { "flac", "audio/flac" },
    { "js", "text/javascript" }
};

const std::string& HandleRequest::GetTypeFromFilePath(const char* filePath)
{
    const char *ptr = filePath, *tag = nullptr;
    int pos = 0;
    while (*ptr) {
        if (*ptr == '.')
            tag = ptr + 1;
        ++ptr;
    }
    return tag
        ? (*contentTypeMap.find(std::string(tag))).second
        : (*contentTypeMap.find(std::string("js"))).second;
}

const std::string& HandleRequest::GetFirstValue(const std::string& key) 
{
    static std::string emptyString = std::string();
    return table.find(key) != table.end()
        ? *table[key].begin()
        : emptyString;
}

void HandleRequest::SendResponse()
{
    auto buffer = std::unique_ptr<char[]>(new char[1 << 10]);
    Reader reader;
    auto path = (*rootPath + GetFirstValue("URI"));
    int fileSize = reader.OpenFile(path.c_str());
    if (fileSize == -1) {
        send(clientSocket, "HTTP/1.1 404 Not Found\r\n", 22, 0);
        return;
    } else {
        std::string text = HttpGenerator()(
            GetTypeFromFilePath(path.c_str()),
            std::to_string(fileSize));
        //printf("generated text:\n%s", text.c_str());
        send(clientSocket, text.c_str(), text.size(), 0);
        while (!feof(reader.GetFilePointer())) {
            int blockSize = reader.ReadData();
            send(clientSocket, reader.GetBufferPointer(), blockSize, 0);
        }
        reader.CloseFile();
    }
}

HandleRequest::~HandleRequest()
{
    closesocket(clientSocket);
}

