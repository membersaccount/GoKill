#include "S_core.h"

constexpr const UINT16 SERVER_PORT = 25500;
constexpr const UINT16 MAX_CLIENT_COUNT = 40;

int main()
{
	try
	{
		ServerCore server;

		server.Run(SERVER_PORT, MAX_CLIENT_COUNT);

		printf("Server Running...\n");
		getchar();

		server.DestroyThread();
	}
	catch (const std::exception&)
	{
		// 로그를 남기고 종료하지 않고 대기한다.
		printf("Exception Occured\n");
		getchar();
	}

	return true;

}