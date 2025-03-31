#include "S_core.h"

ServerCore::ServerCore()
	: recvBuffer(1024), sendBuffer(1024)
{
	InitializeCriticalSection(&cs);
}

ServerCore::~ServerCore()
{
	DeleteCriticalSection(&cs);

	if (serverSock != INVALID_SOCKET)
	{
		closesocket(serverSock);
	}

	WSACleanup();
}

bool ServerCore::Run(const int bindPort, const UINT32 maxClientCount_)
{
	Core::InitSock(&serverSock, 2);
	Core::Server::BindListen(&serverSock, bindPort);

	printf("LookFreeCheck() => %d\n", LookFreeCheck());

	CreateClient(maxClientCount_);

	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKER_THREAD);
	if (IOCPHandle == NULL)
	{
		printf("ServerCore::StartServer() => CreateIoCompletionPort() Error : %d\n", GetLastError());
		return FAIL;
	}

	bool bRet = CreateWorkerThread();
	if (bRet == false)
	{
		printf("ServerCore::StartServer() => CreateWorkerThread() Error\n");
		return FAIL;
	}

	bRet = CreateAccepterThread();
	if (bRet == false)
	{
		printf("ServerCore::StartServer() => CreateAcceptThread() Error\n");
		return FAIL;
	}

	printf("ServerCore::StartServer() => Success\n");
	printf("Server Start\n");

	return SUCCESS;
}

void ServerCore::DestroyThread()
{
	isWorkerRun = false;
	CloseHandle(IOCPHandle);

	for (auto& worker : IOWorkerThreads)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}

	isAccepterRun = false;
	if (accepterThread.joinable())
	{
		accepterThread.join();
	}

	Core::CloseSock(&serverSock);
}

bool ServerCore::LookFreeCheck()
{
	if (isWorkerRun.is_lock_free() && isAccepterRun.is_lock_free())
	{
		return true;
	}

	return false;
}

void ServerCore::CreateClient(const UINT32 maxClientCount_)
{
	clientInfos.reserve(maxClientCount_);

	for (UINT32 i = 0; i < maxClientCount_; ++i)
	{
		clientInfos.emplace_back();
		clientInfos[i].clientIndex = i;
	}
}

bool ServerCore::CreateWorkerThread()
{
	unsigned int threadID = 0;
	for (int i = 0; i < MAX_WORKER_THREAD; ++i)
	{
		IOWorkerThreads.emplace_back(std::thread(&ServerCore::WorkerThread, this));
	}

	printf("ServerCore::CreateWorkerThread() => Success\n");
	return SUCCESS;
}

bool ServerCore::CreateAccepterThread()
{
	accepterThread = std::thread(&ServerCore::AccepterThread, this);

	printf("ServerCore::CreateAcceptThread() => Success\n");
	return SUCCESS;
}

CLIENT_INFO* ServerCore::GetEmptyClientInfo()
{
	for (auto& client : clientInfos)
	{
		if (client.socketClient == INVALID_SOCKET)
		{
			return &client;
		}
	}

	return nullptr;
}

void ServerCore::CloseSocket(CLIENT_INFO* clientInfo_, bool isForce_)
{
	INT32 clientIndex = clientInfo_->clientIndex;

	struct linger stLinger = { 0 };

	if (isForce_ == true)
	{
		stLinger.l_onoff = 1;
		stLinger.l_linger = 0;
	}

	shutdown(clientInfo_->socketClient, SD_BOTH);
	setsockopt(clientInfo_->socketClient, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	closesocket(clientInfo_->socketClient);
	clientInfo_->socketClient = INVALID_SOCKET;

	//OnClose(clientIndex);
}

bool ServerCore::BindIOCP(CLIENT_INFO* clientInfo_)
{
	HANDLE hIOCP = CreateIoCompletionPort((HANDLE)clientInfo_->socketClient, IOCPHandle, (ULONG_PTR)(clientInfo_), 0);
	if (hIOCP == NULL || hIOCP != IOCPHandle)
	{
		printf("ServerCore::BindIOCP() => CreateIoCompletionPort() Error : %d\n", GetLastError());
		return FAIL;
	}

	return SUCCESS;
}

bool ServerCore::BindRecv(CLIENT_INFO* clientInfo_)
{
	DWORD dwFlag = 0;
	DWORD dwRecvSize = 0;

	clientInfo_->stRecvOverlappedEX.wsaBuf.len = MAX_SOCKBUF;
	clientInfo_->stRecvOverlappedEX.wsaBuf.buf = clientInfo_->recvBuf;
	clientInfo_->stRecvOverlappedEX.eIOOperation = IoOperation::RECV;

	int ret = WSARecv(clientInfo_->socketClient,
		&(clientInfo_->stRecvOverlappedEX.wsaBuf),
		1,
		&dwRecvSize,
		&dwFlag,
		(LPWSAOVERLAPPED) & (clientInfo_->stRecvOverlappedEX),
		NULL);

	if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf("ServerCore::BindRecv() => WSARecv() Error : %d\n", WSAGetLastError());
		return FAIL;
	}

	return SUCCESS;
}

void ServerCore::WorkerThread()
{
	CLIENT_INFO* clientInfo = nullptr;
	BOOL bSuccess = FALSE;
	DWORD dwIOSize = 0;
	LPOVERLAPPED lpOverlapped = nullptr;

	printf("WorkerThread Start\n");

	while (isWorkerRun)
	{
		//printf("WorkerThread() => Worker Triggered\n");
		bSuccess = GetQueuedCompletionStatus(IOCPHandle, &dwIOSize, (PULONG_PTR)&clientInfo, &lpOverlapped, INFINITE);

		if (bSuccess == TRUE && dwIOSize == 0 && lpOverlapped == nullptr)
		{
			isWorkerRun = false;
			continue; // break;
		}

		if (lpOverlapped == nullptr)
		{
			continue;
		}

		if (bSuccess == FALSE || (dwIOSize == 0 && bSuccess == TRUE))
		{
			//printf("WorkerThread() => Client Disconnect\n");
			CloseSocket(clientInfo);

			--clientCount;
			connectedClientIndex.remove(clientInfo->clientIndex);

			printf("\n[LOG]: ClientDisconnected, ID = %d, clientIndex = %d, ClientCount = %d, connectedClientIndex Size = %d\n", (int)clientInfo->socketClient, clientInfo->clientIndex, clientCount, (int)connectedClientIndex.size());

			continue;
		}

		OVERLAPPED_EX* pOverlappedEX = (OVERLAPPED_EX*)lpOverlapped;
		if (pOverlappedEX->eIOOperation == IoOperation::RECV)
		{
			
			int workSize = 0;
			int headerSize = sizeof(Packet::Header::DEFAULT);
			int dataSize = 0;
			char tempBuffer[1024];

			int sendCount = 1;
			while ((int)dwIOSize > workSize + headerSize)
			{
				memset(tempBuffer, 0, sizeof(tempBuffer));

				Packet::Header::DEFAULT* headerData = reinterpret_cast<Packet::Header::DEFAULT*>(clientInfo->recvBuf);

				workSize += headerSize;
				dataSize = headerData->size - headerSize;

				if ((int)dwIOSize < workSize + dataSize)
				{
					printf("\n[TrashPacket] Packet Size Error\n");
					
					int* test1 = reinterpret_cast<int*>(clientInfo->recvBuf);
					std::cout << *test1 << std::endl;
					//printf("test1(int) = %d", *test1);
					char* test2 = reinterpret_cast<char*>(clientInfo->recvBuf);
					std::cout << *test2 << std::endl;
					//printf("test2(char) = %s", *test2);

					continue;
				}

				memcpy(tempBuffer, clientInfo->recvBuf, headerSize);

				switch (headerData->type)
				{
				case 0:
				{
					dataSize = sizeof(Packet::Payload::DEBUG);
					workSize += dataSize;

					Packet::Payload::DEBUG* testData = reinterpret_cast<Packet::Payload::DEBUG*>(clientInfo->recvBuf + headerSize);
					printf("Debug: [Recv] Header => Type = %d, Size = %d, Data => X = %.1f, Y = %.1f, Z = %.1f\n", headerData->type, headerData->size, testData->x, testData->y, testData->z);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);

					SendToAll(tempBuffer, headerData->size);
				}
				break;
				case 1:
				{
					dataSize = sizeof(Packet::Payload::NEWPLAYER);
					workSize += dataSize;

					Packet::Payload::NEWPLAYER* testData = reinterpret_cast<Packet::Payload::NEWPLAYER*>(clientInfo->recvBuf + headerSize);
					printf("Debug: [Recv] Header => Type = %d, Size = %d, Data => ID = %d\n", headerData->type, headerData->size, testData->id);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);

					SendToAll(tempBuffer, headerData->size);
				}
				break;
				case 2:
				{
					dataSize = sizeof(Packet::Payload::MOVEMENT);
					workSize += dataSize;

					Packet::Payload::MOVEMENT* testData = reinterpret_cast<Packet::Payload::MOVEMENT*>(clientInfo->recvBuf + headerSize);
					//printf("Debug: [Recv] Header => Type = %d, Size = %d, Data => ID = %d, X = %.1f, Y = %.1f, Z = %.1f, Pitch = %.1f, Yaw = %.1f, Roll = %.1f, VX = %.1f, VY = %.1f, VZ = %.1f\n", headerData->type, headerData->size, testData->id, testData->x, testData->y, testData->z, testData->Pitch, testData->Yaw, testData->Roll, testData->vx, testData->vy, testData->vz);
					printf("[2](%d)", testData->id);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);

					SendToAll(tempBuffer, headerData->size);
				}
				break;
				case 3:
				{
					dataSize = sizeof(Packet::Payload::VOTE_DATA);
					workSize += dataSize;

					Packet::Payload::VOTE_DATA* testData = reinterpret_cast<Packet::Payload::VOTE_DATA*>(clientInfo->recvBuf + headerSize);
					printf("[3Vote](%d-%d)", testData->id, testData->idTarget);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);

					printf("\n[3:Vote]: Enter cs\n");
					EnterCriticalSection(&cs);
					++voteData[testData->idTarget];
					++voteCount;

					if (voteCount >= 5)
					{
						SendVoteResult();
						voteCount = 0;
						for (int i = 0; i < 5;++i)
						{
							voteData[i] = 0;
						}
					}					
					LeaveCriticalSection(&cs);
					printf("[3:Vote]: Leave cs\n");
				}
				break;
				case 4:
				{
					dataSize = sizeof(Packet::Payload::KILL);
					workSize += dataSize;

					Packet::Payload::KILL* testData = reinterpret_cast<Packet::Payload::KILL*>(clientInfo->recvBuf + headerSize);
					printf("[4Kill](%d)", testData->idKill);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);

					SendToAll(tempBuffer, headerData->size);
				}
				break;
				case 5:
				{
					dataSize = sizeof(Packet::Payload::MISSION_PROCESS_DATA);
					workSize += dataSize;

					Packet::Payload::MISSION_PROCESS_DATA* testData = reinterpret_cast<Packet::Payload::MISSION_PROCESS_DATA*>(clientInfo->recvBuf + headerSize);
					printf("[5Miss](%d-%d)", testData->id, testData->missionId);

					memcpy(tempBuffer + headerSize, clientInfo->recvBuf + headerSize, dataSize);
				}
				break;
				default:
					printf("WorkerThread() => Unknown Packet Type\n");
					break;
				}
				++sendCount;
			}

			BindRecv(clientInfo);
		}
		else if (pOverlappedEX->eIOOperation == IoOperation::SEND)
		{
			//printf("[-> Send] bytes = %d\n", dwIOSize);
		}
		else
		{
			printf("WorkerThread() => Unknown Operation, socket(%d)\n", (int)clientInfo->socketClient);
		}
	}
}

void ServerCore::AccepterThread()
{
	SOCKADDR_IN stClientAddr;
	int addrSize = sizeof(SOCKADDR_IN);

	while (isAccepterRun)
	{
		CLIENT_INFO* clientInfo = GetEmptyClientInfo();
		if (clientInfo == nullptr)
		{
			printf("AcceptThread() => Client Full\n");
			return;
		}

		clientInfo->socketClient = accept(serverSock, (SOCKADDR*)&stClientAddr, &addrSize);
		if (clientInfo->socketClient == INVALID_SOCKET)
		{
			printf("AcceptThread() => accept() Error : %d\n", WSAGetLastError());
			continue;
		}

		bool bRet = BindIOCP(clientInfo);
		if (bRet == false)
		{
			printf("AcceptThread() => BindIOCP() Error\n");
			return;
		}

		bRet = BindRecv(clientInfo);
		if (bRet == false)
		{
			printf("AcceptThread() => BindRecv() Error\n");
			return;
		}

		printf("\n========== Client Connect ==========\n");
		printf("AcceptThread() => Client Connect, socket(%d)\n", (int)clientInfo->socketClient);

		//OnConnect(clientInfo->clientIndex);
		{
			char InitClientBuffer[256] = { 0 };
			
			Packet::Header::DEFAULT InitClientHeader;
			InitClientHeader.type = 1;
			InitClientHeader.size = sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::NEWPLAYER);

			Packet::Payload::NEWPLAYER InitClientData;
			InitClientData.id = (int)clientInfo->socketClient;
			InitClientData.index = clientCount;

			printf("Debug: [Send] Header => Type=%d, Size=%d, Data => ID=%d, Index=%d\n", InitClientHeader.type, InitClientHeader.size, InitClientData.id, InitClientData.index);

			memcpy(InitClientBuffer, &InitClientHeader, sizeof(Packet::Header::DEFAULT));
			memcpy(InitClientBuffer + sizeof(Packet::Header::DEFAULT), &InitClientData, sizeof(Packet::Payload::NEWPLAYER));

			Packet::Header::DEFAULT* headerDataDebug = reinterpret_cast<Packet::Header::DEFAULT*>(InitClientBuffer);
			Packet::Payload::NEWPLAYER* payloadDataDebug = reinterpret_cast<Packet::Payload::NEWPLAYER*>(InitClientBuffer + sizeof(Packet::Header::DEFAULT));

			printf("DebugCheck: [Send] Header => Type=%d, Size=%d, Data => ID=%d, Index=%d\n", headerDataDebug->type, headerDataDebug->size, payloadDataDebug->id, payloadDataDebug->index);

			SendWithBufferData(clientInfo, InitClientBuffer, sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::NEWPLAYER));
		}

		++clientCount;

		connectedClientIndex.push_back(clientInfo->clientIndex);
	}
}

bool ServerCore::SendData(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_)
{
	DWORD dwSendSize = 0;

	clientInfo_->stSendOverlappedEX.wsaBuf.len = bytes_;
	clientInfo_->stSendOverlappedEX.wsaBuf.buf = clientInfo_->sendBuf;
	clientInfo_->stSendOverlappedEX.eIOOperation = IoOperation::SEND;

	int ret = WSASend(clientInfo_->socketClient,
		&(clientInfo_->stSendOverlappedEX.wsaBuf),
		1,
		&dwSendSize,
		0,
		(LPWSAOVERLAPPED) & (clientInfo_->stSendOverlappedEX),
		NULL);

	if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf("ServerCore::SendMsg() => WSASend() Error : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

void ServerCore::SendTest()
{
	printf("Called: SendTest()\n");

	char buffer[256];
	printf("recvBuffer.Read: Result = %d\n", recvBuffer.Read(buffer, recvBuffer.GetUsedDataSize()));
	printf("sendBuffer.Write: Result = %d\n", sendBuffer.Write(buffer, recvBuffer.GetUsedDataSize()));

	int bufferSize = sendBuffer.GetUsedDataSize();
	sendBuffer.Read(buffer, sendBuffer.GetUsedDataSize());
	
	CLIENT_INFO* clientInfo = nullptr;

	for (auto& index : connectedClientIndex)
	{
		clientInfo = &clientInfos[index];
		if (clientInfos[index].socketClient == INVALID_SOCKET)
		{
			printf("SendTest(), Index = %d, Info Index = %d, Socket = %d\n", index, clientInfos[index].clientIndex, (int)clientInfos[index].socketClient);
		}

		memcpy(clientInfo->sendBuf, buffer, bufferSize);

		SendData(clientInfo, buffer, bufferSize);
	}

}

bool ServerCore::SendRecvBuffer(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_)
{
	DWORD dwSendSize = 0;

	memcpy(clientInfo_->sendBuf, data_, bytes_);

	clientInfo_->stSendOverlappedEX.wsaBuf.len = bytes_;
	clientInfo_->stSendOverlappedEX.wsaBuf.buf = clientInfo_->sendBuf;
	clientInfo_->stSendOverlappedEX.eIOOperation = IoOperation::SEND;

	int ret = WSASend(clientInfo_->socketClient,
		&(clientInfo_->stSendOverlappedEX.wsaBuf),
		1,
		&dwSendSize,
		0,
		(LPWSAOVERLAPPED) & (clientInfo_->stSendOverlappedEX),
		NULL);

	if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf("ServerCore::SendMsg() => WSASend() Error : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

void ServerCore::SendToAll(const char* data_, UINT32 bytes_)
{
	CLIENT_INFO* clientInfo = nullptr;

	for (auto& client : clientInfos)
	{
		if (client.socketClient == INVALID_SOCKET)
		{
			continue;
		}

		clientInfo = &client;
		SendWithBufferData(clientInfo, data_, bytes_);
	}
}

bool ServerCore::SendWithBufferData(CLIENT_INFO* clientInfo_, const char* data_, UINT32 bytes_)
{
	DWORD dwSendSize = 0;

	memcpy(clientInfo_->sendBuf, data_, bytes_);

	clientInfo_->stSendOverlappedEX.wsaBuf.len = bytes_;
	clientInfo_->stSendOverlappedEX.wsaBuf.buf = clientInfo_->sendBuf;
	clientInfo_->stSendOverlappedEX.eIOOperation = IoOperation::SEND;

	int ret = WSASend(clientInfo_->socketClient,
		&(clientInfo_->stSendOverlappedEX.wsaBuf),
		1,
		&dwSendSize,
		0,
		(LPWSAOVERLAPPED) & (clientInfo_->stSendOverlappedEX),
		NULL);

	if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf("ServerCore::SendMsg() => WSASend() Error : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

void ServerCore::SendVoteResult()
{
	int maxVote = 0;
	int resultIndex = -1;
	bool sameVote = false;

	for (int i = 0; i < 5; ++i)
	{
		if (voteData[i] == maxVote)
		{
			sameVote = true;
		}

		if (voteData[i] > maxVote)
		{
			sameVote = false;
			maxVote = voteData[i];
			resultIndex = i;
		}
	}

	if (sameVote)
	{
		resultIndex = -1;
	}

	char voteResult[64] = { 0 };

	Packet::Header::DEFAULT voteResultHeader;
	voteResultHeader.type = 4;
	voteResultHeader.size = sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::KILL);

	Packet::Payload::KILL voteResultData;
	voteResultData.idKill = resultIndex;

	printf("Debug: [Send] Header => Type = %d, Size = %d, Data => KillID = %d\n", voteResultHeader.type, voteResultHeader.size, voteResultData.idKill);

	memcpy(voteResult, &voteResultHeader, sizeof(Packet::Header::DEFAULT));
	memcpy(voteResult + sizeof(Packet::Header::DEFAULT), &voteResultData, sizeof(Packet::Payload::KILL));

	Packet::Header::DEFAULT* headerDataDebug = reinterpret_cast<Packet::Header::DEFAULT*>(voteResult);
	Packet::Payload::KILL* payloadDataDebug = reinterpret_cast<Packet::Payload::KILL*>(voteResult + sizeof(Packet::Header::DEFAULT));

	printf("DebugCheck: [Send] Header => Type = %d, Size = %d, Data => ID = %d\n", headerDataDebug->type, headerDataDebug->size, payloadDataDebug->idKill);

	SendToAll(voteResult, sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::KILL));
}
