#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <Ws2tcpip.h>
#include <Windows.h>

#include <iostream>

#include "shPacketHandler.h"

constexpr bool SUCCESS = true;
constexpr bool FAIL = false;

int SHDummyTest();

namespace Core
{
	bool InitSock(SOCKET* sock_, int type_);
	void CloseSock(SOCKET* sock_);

	namespace Client
	{
		bool InitConnect(SOCKET* sock_);
	}

	namespace Server
	{
		bool BindListen(SOCKET* sock_, const int bindPort);
	}

	namespace Logic
	{
		bool SockTCP(SOCKET* sock_);
		bool SockIOCP(SOCKET* sock_);
	}
}

namespace Buffer
{
	class RecvBuffer
	{
	public:
		explicit RecvBuffer(UINT32 size_);
		~RecvBuffer();

		bool Write(const char* data_, UINT32 bytes_);
		bool Read(char* data_, UINT32 bytes_);

		UINT32 GetFreeDataSize() const;
		UINT32 GetUsedDataSize() const;


	public:
		mutable CRITICAL_SECTION cs;

	private:
		UINT32 bufferSize = 0;
		UINT32 writeIndex = 0;
		UINT32 readIndex = 0;
		std::vector<char> buffer;
	};

	class SendBuffer
	{
	public:
		explicit SendBuffer(UINT32 size_);
		~SendBuffer();

		bool Write(const char* data_, UINT32 bytes_);
		bool Read(char* data_, UINT32 bytes_);

		UINT32 GetFreeDataSize() const;
		UINT32 GetUsedDataSize() const;

		std::vector<char> GetBuffer() const;

	public:
		mutable CRITICAL_SECTION cs;

	private:
		UINT32 bufferSize = 0;
		UINT32 writeIndex = 0;
		UINT32 readIndex = 0;
		std::vector<char> buffer;
	};
}

namespace Debug
{
	HWND CreateNewConsole();
	void DestroyConsole(HWND consoleHandle_);
}