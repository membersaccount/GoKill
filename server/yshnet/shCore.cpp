#include "pch.h"
#include "shCore.h"

int SHDummyTest()
{
	return 100;
}

bool Core::InitSock(SOCKET* sock_, int type_)
{
	bool result = FAIL;

	switch (type_)
	{
	case 1:
		result = Logic::SockTCP(sock_);
		break;
	case 2:
		result = Logic::SockIOCP(sock_);
		break;
	default:
		break;
	}

	return result;
}

void Core::CloseSock(SOCKET* sock_)
{
	closesocket(*sock_);
	WSACleanup();
}

bool Core::Client::InitConnect(SOCKET* sock_)
{
	SOCKADDR_IN clientAddr;

	ZeroMemory(&clientAddr, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_port = htons(25500);

	int retConnect = connect(*sock_, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
	if (retConnect < 0)
	{
		return FAIL;
	}

	return SUCCESS;
}

bool Core::Server::BindListen(SOCKET* sock_, const int bindPort)
{
	SOCKADDR_IN stServerAddr;
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(bindPort);
	stServerAddr.sin_addr.S_un.S_addr = htons(INADDR_ANY);

	int ret = bind(*sock_, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (ret != 0)
	{
		printf("ServerCore::BindAndListen() => bind() Error : %d\n", WSAGetLastError());
		return FAIL;
	}

	ret = listen(*sock_, 5);
	if (ret != 0)
	{
		printf("ServerCore::BindAndListen() => listen() Error : %d\n", WSAGetLastError());
		return FAIL;
	}

	printf("ServerCore::BindAndListen() => Success\n");
	return SUCCESS;
}

bool Core::Logic::SockTCP(SOCKET* sock_)
{
	WSADATA WsaData;

	int RetStartup = WSAStartup(MAKEWORD(2, 2), &WsaData);
	if (0 != RetStartup)
	{
		printf("Error = WSAStartup : %d\n", WSAGetLastError());
		return FAIL;
	}

	*sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (*sock_ == INVALID_SOCKET)
	{
		printf("Error = socket : %d\n", WSAGetLastError());
		return FAIL;
	}

	printf("ServerCore::InitSocket() => Success\n");
	return SUCCESS;
}

bool Core::Logic::SockIOCP(SOCKET* sock_)
{
	WSADATA wsaData;

	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		printf("ServerCore::InitSocket() => WSAStartup() Error : %d\n", WSAGetLastError());
		return FAIL;
	}

	*sock_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (*sock_ == INVALID_SOCKET)
	{
		printf("ServerCore::InitSocket() => WSASocket() Error : %d\n", WSAGetLastError());
		return FAIL;
	}

	printf("ServerCore::InitSocket() => Success\n");
	return SUCCESS;
}


Buffer::RecvBuffer::RecvBuffer(UINT32 size_)
	: bufferSize(size_), buffer(), writeIndex(0), readIndex(0)
{
	buffer.resize(bufferSize);
	InitializeCriticalSection(&cs);
}

Buffer::RecvBuffer::~RecvBuffer()
{
	DeleteCriticalSection(&cs);
}

bool Buffer::RecvBuffer::Write(const char* data_, UINT32 bytes_)
{
	EnterCriticalSection(&cs);

	if (GetFreeDataSize() < bytes_)
	{
		LeaveCriticalSection(&cs);
		std::cerr << "Write failed: Not enough space" << std::endl;
		return FAIL;
	}

	for (UINT32 i = 0; i < bytes_; ++i)
	{
		buffer[writeIndex] = data_[i];
		writeIndex = (++writeIndex) % bufferSize;
	}

	LeaveCriticalSection(&cs);

	return SUCCESS;
}

bool Buffer::RecvBuffer::Read(char* data_, UINT32 bytes_)
{
	EnterCriticalSection(&cs);

	if (GetUsedDataSize() < bytes_)
	{
		LeaveCriticalSection(&cs);
		std::cerr << "Read failed: Not enough data" << std::endl;
		return FAIL;
	}

	for (UINT32 i = 0; i < bytes_; ++i)
	{
		data_[i] = buffer[readIndex];
		readIndex = (readIndex + 1) % bufferSize;
	}

	LeaveCriticalSection(&cs);

	return SUCCESS;
}

UINT32 Buffer::RecvBuffer::GetFreeDataSize() const
{
	EnterCriticalSection(&cs);

	UINT32 freeSize;
	if (writeIndex >= readIndex)
	{
		freeSize = bufferSize - writeIndex + readIndex;
	}
	else
	{
		freeSize = readIndex - writeIndex;
	}

	LeaveCriticalSection(&cs);

	return freeSize;
}

UINT32 Buffer::RecvBuffer::GetUsedDataSize() const
{
	EnterCriticalSection(&cs);

	UINT32 usedSize;
	if (writeIndex >= readIndex)
	{
		usedSize = writeIndex - readIndex;
	}
	else
	{
		usedSize = bufferSize - readIndex + writeIndex;
	}

	LeaveCriticalSection(&cs);

	return usedSize;
}

Buffer::SendBuffer::SendBuffer(UINT32 size_)
	: bufferSize(size_), buffer(), writeIndex(0), readIndex(0)
{
	buffer.resize(size_);
	InitializeCriticalSection(&cs);
}

Buffer::SendBuffer::~SendBuffer()
{
	DeleteCriticalSection(&cs);
}

bool Buffer::SendBuffer::Write(const char* data_, UINT32 bytes_)
{
	EnterCriticalSection(&cs);

	if (GetFreeDataSize() < bytes_)
	{
		LeaveCriticalSection(&cs);
		std::cerr << "Write failed: Not enough space" << std::endl;
		return FAIL;
	}

	for (UINT32 i = 0; i < bytes_; ++i)
	{
		buffer[writeIndex] = data_[i];
		writeIndex = (writeIndex + 1) % bufferSize;
	}

	LeaveCriticalSection(&cs);

	return SUCCESS;
}

bool Buffer::SendBuffer::Read(char* data_, UINT32 bytes_)
{
	EnterCriticalSection(&cs);

	if (GetUsedDataSize() < bytes_)
	{
		LeaveCriticalSection(&cs);
		std::cerr << "Read failed: Not enough data" << std::endl;
		return FAIL;
	}

	for (UINT32 i = 0; i < bytes_; ++i)
	{
		data_[i] = buffer[readIndex];
		readIndex = (readIndex + 1) % bufferSize;
	}

	LeaveCriticalSection(&cs);

	return false;
}

UINT32 Buffer::SendBuffer::GetFreeDataSize() const
{
	EnterCriticalSection(&cs);

	UINT32 freeSize;
	if (writeIndex >= readIndex)
	{
		freeSize = bufferSize - writeIndex + readIndex;
	}
	else
	{
		freeSize = readIndex - writeIndex;
	}

	LeaveCriticalSection(&cs);

	return freeSize;
}

UINT32 Buffer::SendBuffer::GetUsedDataSize() const
{
	EnterCriticalSection(&cs);

	UINT32 usedSize;
	if (writeIndex >= readIndex)
	{
		usedSize = writeIndex - readIndex;
	}
	else
	{
		usedSize = bufferSize - readIndex + writeIndex;
	}

	LeaveCriticalSection(&cs);

	return usedSize;
}

std::vector<char> Buffer::SendBuffer::GetBuffer() const
{
	return buffer;
}

HWND Debug::CreateNewConsole()
{
	if (AllocConsole())
	{
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);
		std::cout.clear();
		std::cerr.clear();
		std::clog.clear();
		std::wcout.clear();
		std::wcerr.clear();
		std::wclog.clear();

		std::cout << "=== Console window created ===\n";

		return GetConsoleWindow();
	}

	std::cout << "=== Failed to create console window ===\n";
	return nullptr;
}

void Debug::DestroyConsole(HWND consoleHandle_)
{
	if (consoleHandle_ == nullptr)
	{
		return;
	}

	PostMessage(consoleHandle_, WM_CLOSE, 0, 0);
	consoleHandle_ = nullptr;
}
