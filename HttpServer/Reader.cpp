#include "Reader.h"

const int blockSize = 1 << 16;

const char* Reader::GetBufferPointer()
{
	return buffer;
}
int Reader::OpenFile(const char* filePath)
{
    if (filePointer)
        return -1;
    filePointer = fopen(filePath, "rb");
    if (!filePointer)
        return -1; //printf("Cannot open: %s\n", filePath), -1;
    fseek(filePointer, 0, SEEK_END);
    int fileLength = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);
    return //printf("Successfully open: %s, size: %d\n", filePath, fileLength),
        fileLength;
}
void Reader::CloseFile()
{
	if (filePointer)
	{
		//printf("Close file\n");
		fclose(filePointer);
	}
	filePointer = nullptr;
}
int Reader::ReadData()
{
    if (!filePointer)
        return -1;
    //memset(buffer, 0, blockSize);
    int fileSize = fread(buffer, sizeof(char), blockSize, filePointer);
    buffer[fileSize] = 0;
    int errCode = ferror(filePointer);
    if (errCode)
        return -1;
    //printf("Read file error, code: %d\n\n", errCode), -1;
    return fileSize;
}
FILE* Reader::GetFilePointer()
{
	return filePointer;
}
Reader::Reader()
{
	buffer = new char[blockSize];
	filePointer = nullptr;
}
Reader::~Reader()
{
	delete[]buffer;
}