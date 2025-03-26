#include "Network/SHNetCore.h"
#include "shPacketHandler.h"

SHNetCore::SHNetCore()
	: recvBuffer(1024), sendBuffer(1024)
{
}

SHNetCore::~SHNetCore()
{
	WSACleanup();
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
	if (recvThread.joinable())
	{
		recvThread.join();
	}

	Core::CloseSock(&clientSocket);
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

	while (true)
	{
		int recvSize = recv(clientSocket, buffer, bufferSize, 0);

		if (0 >= recvSize)
		{
			std::cout << "Server End\n" << std::endl;
			break;
		}

		recvBuffer.Write(buffer, recvSize);
		printf("Recv Size = %d, Used Size = %d\n", recvSize, recvBuffer.GetUsedDataSize());

		// tick
		while (recvBuffer.GetUsedDataSize() >= sizeof(Packet::Header::DEFAULT))
		{
			//printf("Packet Heandler\n");

			char cpyBuffer[1024];
			int cpyBufferSize = sizeof(Packet::Header::DEFAULT);

			recvBuffer.Read(cpyBuffer, cpyBufferSize);

			Packet::Header::DEFAULT* header = reinterpret_cast<Packet::Header::DEFAULT*>(cpyBuffer);

			// lookup table(lib) + callback(UE)
			switch (header->type)
			{
			case 0:
			{
				recvBuffer.Read(cpyBuffer + sizeof(Packet::Header::DEFAULT), header->size - sizeof(Packet::Header::DEFAULT));
				Packet::Payload::DEBUG* testData = reinterpret_cast<Packet::Payload::DEBUG*>(cpyBuffer + sizeof(Packet::Header::DEFAULT));

				printf("Recv Packet: Header Type=%d, Size=%d\n", header->type, header->size);
				printf("Recv Packet: Data X=%f, Y=%f, Z=%f\n", testData->x, testData->y, testData->z);
			}
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
