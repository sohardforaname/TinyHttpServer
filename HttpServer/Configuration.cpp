#include "configuration.h"

/*
* 读取配置文件
* @param 配置文件路径
* @return
*/
void Configuration::ReadConfigurationFile(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb");
	if (!fp)
	{
		printf("Cannot read configuration file\n");
		exit(-1);
	}
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    auto buffer = std::unique_ptr<char[]>(new char[len + 1]);
    int fileSize = fread(buffer.get(), 1, len, fp);
    buffer[fileSize] = 0;
    table = ConfigurationParser()(buffer.get(), fileSize);
	fclose(fp);
}

Configuration::Configuration() { }

Configuration::~Configuration() { }

std::string Configuration::GetValue(const std::string& key)
{
    return std::move(
        table.find(key) != table.end() ? table[key] : std::string()
    );
}