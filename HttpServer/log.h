#pragma once

#include <cstdio>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <ctime>

class Log
{
private:
public:
	void PrintTime();
	void PrintString(const std::string);
	void PrintMap(
		const std::unordered_map<std::string, 
		std::unordered_set<std::string>> mp
	);
	void PrintSet(const std::unordered_set<std::string> st);
	FILE* GetLogFilePointer();
	Log();
	~Log();
};