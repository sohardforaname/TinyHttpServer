#include "Server.h"

int main()
{
    Server::GetInstance().SetMaxThread(10);
    Server::GetInstance().Loop();
	return 0;
}