#include "Network/SHNetCore.h"
#include "shPacketHandler.h"

#include "Network/SHNetGameInstance.h"
#include "Network/SHNetPlayerController.h"

SHNetCore* SHNetCore::networkSystem = nullptr;

SHNetCore::SHNetCore()
	: recvBuffer(1024), sendBuffer(1024)
{
}

SHNetCore::~SHNetCore()
{
	WSACleanup();
}

SHNetCore* SHNetCore::GetInstance()
{
	if (networkSystem == nullptr)
	{
		networkSystem = new SHNetCore();
	}

	return networkSystem;
}

bool SHNetCore::Run()
{
	if (Core::InitSock(&clientSocket, 1))
	{
		printf("InitSock() => Success\n");
	}
	else
	{
		printf("InitSock() => Fail\n");
		return FAIL;
	}

	if (Core::Client::InitConnect(&clientSocket))
	{
		printf("InitConnect() => Success\n");
	}
	else
	{
		printf("InitConnect() => Fail\n");
		return FAIL;
	}

	recvThread = std::thread(&SHNetCore::RecvThread, this);

	return SUCCESS;
}

void SHNetCore::DestroyThread()
{
	isRecvRun = false;

	closesocket(clientSocket);

	if (recvThread.joinable())
	{
		recvThread.join();
	}

	Core::CloseSock(&clientSocket);
}

SOCKET* SHNetCore::GetSocket()
{
	return &clientSocket;
}

void SHNetCore::SetGameInstance(USHNetGameInstance* gameInstance_)
{
	cachedGameInstance = gameInstance_;
	++SetGameInstanceCount;
}

void SHNetCore::SetController(ASHNetPlayerController* controller_)
{
	cachedController = controller_;
}

void SHNetCore::SendTestMsg()
{
	Packet::Header::DEFAULT testPacketHeader;
	testPacketHeader.type = 0;
	testPacketHeader.size = sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::DEBUG);

	Packet::Payload::DEBUG testPacketData;
	testPacketData.x = 1 + testValue;
	testPacketData.y = 2 + testValue;
	testPacketData.z = 3 + testValue;

	++testValue;


	char buffer[1024];

	sendBuffer.Write((char*)&testPacketHeader, sizeof(Packet::Header::DEFAULT));
	sendBuffer.Write((char*)&testPacketData, sizeof(Packet::Payload::DEBUG));

	int bufferSize = sendBuffer.GetUsedDataSize();


	sendBuffer.Read(buffer, bufferSize);

	printf("[Send] bufferSize = %d, send result = %d\n", bufferSize, send(clientSocket, buffer, bufferSize, 0));
}

void SHNetCore::RecvThread()
{
	constexpr const int bufferSize = 1024;
	char buffer[bufferSize];

	while (isRecvRun)
	{
		int recvSize = recv(clientSocket, buffer, bufferSize, 0);

		if (0 >= recvSize)
		{
			std::cout << "Server End\n" << std::endl;
			break;
		}

		recvBuffer.Write(buffer, recvSize);
		printf("Recv Size = %d, Used Size = %d\n", recvSize, recvBuffer.GetUsedDataSize());

		while (recvBuffer.GetUsedDataSize() >= sizeof(Packet::Header::DEFAULT))
		{
			char cpyBuffer[1024];
			int cpyBufferSize = sizeof(Packet::Header::DEFAULT);

			recvBuffer.Read(cpyBuffer, cpyBufferSize);

			Packet::Header::DEFAULT* header = reinterpret_cast<Packet::Header::DEFAULT*>(cpyBuffer);

			switch (header->type)
			{
			case 0:
			{
				printf("[Debug]: case 0: Try to Read Packet: Header Type=%d, Size=%d\n", header->type, header->size);

				recvBuffer.Read(cpyBuffer + sizeof(Packet::Header::DEFAULT), header->size - sizeof(Packet::Header::DEFAULT));
				Packet::Payload::DEBUG* testData = reinterpret_cast<Packet::Payload::DEBUG*>(cpyBuffer + sizeof(Packet::Header::DEFAULT));

				printf("Recv Packet: Header Type=%d, Size=%d, Data X=%f, Y=%f, Z=%f\n", header->type, header->size, testData->x, testData->y, testData->z);
			}
			break;
			case 1:
			{
				printf("[Debug]: case 1: Try to Read Packet: Header Type=%d, Size=%d\n", header->type, header->size);

				recvBuffer.Read(cpyBuffer + sizeof(Packet::Header::DEFAULT), header->size - sizeof(Packet::Header::DEFAULT));
				Packet::Payload::NEWPLAYER* newPlayerData = reinterpret_cast<Packet::Payload::NEWPLAYER*>(cpyBuffer + sizeof(Packet::Header::DEFAULT));

				printf("Recv Packet: Header Type=%d, Size=%d, Data ID=%d, Index=%d\n", header->type, header->size, newPlayerData->id, newPlayerData->index);

				cachedGameInstance->clientID = newPlayerData->id;
                //cachedGameInstance->baseIndex = newPlayerData->index;
                cachedController->updateIndex = newPlayerData->index;
			}
			break;
			case 2:
			{
				printf("[Debug]: case 2: Try to Read Packet: Header Type=%d, Size=%d\n", header->type, header->size);

				recvBuffer.Read(cpyBuffer + sizeof(Packet::Header::DEFAULT), header->size - sizeof(Packet::Header::DEFAULT));
				Packet::Payload::MOVEMENT* movementData = reinterpret_cast<Packet::Payload::MOVEMENT*>(cpyBuffer + sizeof(Packet::Header::DEFAULT));


				printf("Recv Packet: Header Type=%d, Size=%d, Data ID=%d, X=%f, Y=%f, Z=%f, Pitch=%f, Yaw=%f, Roll=%f, Vx=%f, Vy=%f, Vz=%f\n",
					header->type, header->size, movementData->id, movementData->x, movementData->y, movementData->z, movementData->Pitch, movementData->Yaw, movementData->Roll, movementData->vx, movementData->vy, movementData->vz);

				if (cachedController == nullptr)
				{
					continue;
				}

				if (cachedController->cachedID == movementData->id)
				{
					continue;
				}

				std::lock_guard<std::mutex> lock(cachedController->movementMutex);
                cachedController->movementWorks.push(*movementData);
			}
			break;
            case 3:
                printf("Recv Packet type (VOTE_DATA) not allowed");
            break;
            case 4:
            {
                printf("[Debug]: case 4: Try to Read Packet: Header Type=%d, Size=%d\n", header->type, header->size);

                recvBuffer.Read(cpyBuffer + sizeof(Packet::Header::DEFAULT), header->size - sizeof(Packet::Header::DEFAULT));
                Packet::Payload::KILL* killData = reinterpret_cast<Packet::Payload::KILL*>(cpyBuffer + sizeof(Packet::Header::DEFAULT));

                printf("Recv Packet: Header Type=%d, Size=%d, KilledID=%d\n", header->type, header->size, killData->idKill);

                std::lock_guard<std::mutex> lock(cachedController->movementMutex);
                cachedController->killWorks.push(killData->idKill);
            }
            break;
            case 5:
                printf("Recv Packet type (MISSION_PROCESS_DATA) not allowed");
            break;
			default:
				printf("Unknown Packet Type\n");
				break;
			}

			printf("recvBuffer.GetUsedDataSize(), FreeSize = %d, %d\n", recvBuffer.GetUsedDataSize(), recvBuffer.GetFreeDataSize());
			printf("========================================\n");
		}
	}
}
