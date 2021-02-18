#include "Log.h"

static FILE* logfp;
void Log::PrintTime()
{
	time_t curTime;
	time(&curTime);
	fprintf(logfp, "%s", ctime(&curTime));
}
void Log::PrintString(const std::string str)
{
	printf("%s", str.c_str());
}
void Log::PrintMap(
	const std::unordered_map<std::string,
	std::unordered_set<std::string>> mp
)
{
	for (auto& iter : mp)
	{
		printf("%s:", iter.first.c_str());
		PrintSet(iter.second);
	}
}
void Log::PrintSet(const std::unordered_set<std::string> st)
{
	auto back = --st.end();
	for (auto iter = st.begin(); iter != st.end(); ++iter)
		printf("%s%c", (*iter).c_str(), ",\n"[iter == back]);
}
FILE* Log::GetLogFilePointer()
{
	return logfp;
}
Log::Log()
{
	if (!logfp)
		logfp = fopen("/log.txt", "a+");
}
Log::~Log()
{
	fclose(logfp);
}