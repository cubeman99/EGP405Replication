
#include "Server.h"


int main()
{
	Server* server = Server::GetInstance();
	server->RunServer();
	system("pause");
	return 0;
}

