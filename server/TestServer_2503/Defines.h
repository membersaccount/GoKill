#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

const UINT32 MAX_SOCKBUF = 256;
const UINT32 MAX_WORKER_THREAD = 8;

enum class IoOperation
{
	RECV,
	SEND,
};

struct OVERLAPPED_EX
{
	WSAOVERLAPPED wsaOverlapped;
	SOCKET socketClient;
	WSABUF wsaBuf;
	IoOperation eIOOperation;
};

struct CLIENT_INFO
{
	INT32 clientIndex;
	SOCKET socketClient;
	OVERLAPPED_EX stRecvOverlappedEX;
	OVERLAPPED_EX stSendOverlappedEX;
	char recvBuf[MAX_SOCKBUF];
	char sendBuf[MAX_SOCKBUF];

	CLIENT_INFO()
	{
		clientIndex = -1;
		socketClient = INVALID_SOCKET;
		memset(&stRecvOverlappedEX, 0, sizeof(OVERLAPPED_EX));
		memset(&stSendOverlappedEX, 0, sizeof(OVERLAPPED_EX));
		memset(recvBuf, 0, sizeof(recvBuf));
		memset(sendBuf, 0, sizeof(sendBuf));
	}
};
