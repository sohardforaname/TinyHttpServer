#pragma once
#include <cstdio>
#include <cstring>

class Reader
{
private:
	char* buffer;
	FILE* filePointer;

public:
	const char* GetBufferPointer();
	int OpenFile(const char* filePath);
	void CloseFile();
	int ReadData();
	FILE* GetFilePointer();
	Reader();
	~Reader();
};