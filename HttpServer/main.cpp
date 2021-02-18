#include "Server.h"

int main()
{
    Server::GetInstance().SetMaxThread(5);
    Server::GetInstance().Loop();
	return 0;
}