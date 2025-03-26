#pragma once

#include "CoreMinimal.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>

#include <thread>
#include <Windows.h>
#include <atomic>

#include "shCore.h"

class ACCESS_API SHNetCore
{
public:
	SHNetCore();
	~SHNetCore();

	bool Run();
	void DestroyThread();

	void SendTestMsg();

private:
	void RecvThread();

private:
	SOCKET clientSocket;

	std::thread recvThread;
	std::atomic<bool> isRecvRun = true;

	Buffer::RecvBuffer recvBuffer;
	Buffer::SendBuffer sendBuffer;

	float testValue = 0.f;
};
