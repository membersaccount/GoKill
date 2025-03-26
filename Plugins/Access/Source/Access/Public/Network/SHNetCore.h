#pragma once

#include "CoreMinimal.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>

#include <thread>
#include <Windows.h>
#include <atomic>

#include "shCore.h"

class USHNetGameInstance;
class ASHNetPlayerController;

class ACCESS_API SHNetCore
{
public:
	SHNetCore();
	SHNetCore(const SHNetCore&) = delete;
	SHNetCore& operator=(const SHNetCore&) = delete;
	~SHNetCore();

	static SHNetCore* GetInstance();

	bool Run();
	void DestroyThread();

	SOCKET* GetSocket();
	void SetGameInstance(USHNetGameInstance* gameInstance_);
	void SetController(ASHNetPlayerController* controller_);

	void SendTestMsg();

private:
	void RecvThread();

public:
	Buffer::RecvBuffer recvBuffer;
	Buffer::SendBuffer sendBuffer;

	int SetGameInstanceCount = 0;

private:
	static SHNetCore* networkSystem;

	SOCKET clientSocket;

	std::thread recvThread;
	std::atomic<bool> isRecvRun = true;

	USHNetGameInstance* cachedGameInstance = nullptr;
	ASHNetPlayerController* cachedController = nullptr;

	float testValue = 0.f;
};
