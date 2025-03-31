#pragma once

#include "Defines.h"
#pragma comment(lib, "ws2_32.lib")
#include <thread>
#include <vector>
#include <list>
#include <unordered_map>
#include <Windows.h>
#include <atomic>
#include "shCore.h"

class ServerCore
{
public:
	ServerCore();
	~ServerCore();

public:
	bool Run(const int bindPort, const UINT32 maxClientCount_);
	void DestroyThread();

private:
	bool LookFreeCheck();

	void CreateClient(const UINT32 maxClientCount_);
	bool CreateWorkerThread();
	bool CreateAccepterThread();

	CLIENT_INFO* GetEmptyClientInfo();
	void CloseSocket(CLIENT_INFO* clientInfo_, bool isForce_ = false);

private:
	bool BindIOCP(CLIENT_INFO* clientInfo_);
	bool BindRecv(CLIENT_INFO* clientInfo_);
	void WorkerThread();
	void AccepterThread();

private:
	bool SendData(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_);

	void SendTest();

	bool SendRecvBuffer(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_);
	void SendToAll(const char* data_, UINT32 bytes_);

	bool SendWithBufferData(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_);

	void SendVoteResult();

private:
	SOCKET serverSock;
	HANDLE IOCPHandle = INVALID_HANDLE_VALUE;
	CRITICAL_SECTION cs;

	std::vector<std::thread> IOWorkerThreads;
	std::thread accepterThread;
	std::atomic<bool> isWorkerRun = true;
	std::atomic<bool> isAccepterRun = true;

	std::vector<CLIENT_INFO> clientInfos;
	int clientCount = 0;
	std::list<int> connectedClientIndex;

	Buffer::RecvBuffer recvBuffer;
	Buffer::SendBuffer sendBuffer;

	int voteData[5] = { 0 };
	int voteCount = 0;
};