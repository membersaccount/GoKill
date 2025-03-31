#include "Network/SHNetPlayerController.h"
#include "Network/SHNetGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"

ASHNetPlayerController::ASHNetPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (OtherCharacterClass.Succeeded())
	{
		otherCharacter = OtherCharacterClass.Class;
	}
    ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass0(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_Char0Red.BP_Char0Red_C'"));
    if (OtherCharacterClass0.Succeeded())
    {
        otherCharacters.Add(OtherCharacterClass0.Class);
    }
    ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass1(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_Char1Green.BP_Char1Green_C'"));
    if (OtherCharacterClass1.Succeeded())
    {
        otherCharacters.Add(OtherCharacterClass1.Class);
    }
    ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass2(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_Char2Blue.BP_Char2Blue_C'"));
    if (OtherCharacterClass2.Succeeded())
    {
        otherCharacters.Add(OtherCharacterClass2.Class);
    }
    ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass3(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_Char3Yellow.BP_Char3Yellow_C'"));
    if (OtherCharacterClass3.Succeeded())
    {
        otherCharacters.Add(OtherCharacterClass3.Class);
    }
    ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass4(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_Char4Black.BP_Char4Black_C'"));
    if (OtherCharacterClass4.Succeeded())
    {
        otherCharacters.Add(OtherCharacterClass4.Class);
    }
}

void ASHNetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	cachedNetwork = GetWorld()->GetGameInstance<USHNetGameInstance>()->network;
	cachedNetwork->Run();
	cachedSocket = GetWorld()->GetGameInstance<USHNetGameInstance>()->network->GetSocket();
	cachedNetwork->SetGameInstance(GetWorld()->GetGameInstance<USHNetGameInstance>());
	cachedID = GetWorld()->GetGameInstance<USHNetGameInstance>()->clientID;
    //baseIndex = GetWorld()->GetGameInstance<USHNetGameInstance>()->baseIndex;
	cachedNetwork->SetController(this);

    //FActorSpawnParameters spawnParams;
    //spawnParams.Owner = this;
    //spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //for (int i = 0; i < 5; i++)
    //{
    //    FVector Location = GetPawn()->GetActorLocation() + FVector(100.f * i, 0.f, 0.f);
    //    GetWorld()->SpawnActor<ACharacter>(otherCharacters[i], Location, GetPawn()->GetActorRotation(), spawnParams);
    //}

    //GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, TEXT("Character Spawned"));
}

void ASHNetPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (cachedID < 1)
	{
		cachedID = GetWorld()->GetGameInstance<USHNetGameInstance>()->clientID;
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, TEXT("ID is -1"));
		return;
	}

	SendSelfMovement();
	UpdateOtherPlayerMovement();
    KilledPlayer();

	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Magenta, FString::Printf(TEXT("Socket: %d, SelfID: %d"), static_cast<int>(*cachedSocket), cachedID));
	GEngine->AddOnScreenDebugMessage(3, 30.f, FColor::Cyan, FString::Printf(TEXT("OtherCharacters: %d"), otherPlayers.size()));
}

void ASHNetPlayerController::Destroyed()
{
	Super::Destroyed();

	cachedNetwork->DestroyThread();
	closesocket(*cachedSocket);
	WSACleanup();
}

void ASHNetPlayerController::SendSelfMovement()
{
	header.type = 2;
	header.size = sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::MOVEMENT);
	movement.id = cachedID;
    movement.index = updateIndex;

	FVector location = GetPawn()->GetActorLocation();
	movement.x = location.X;
	movement.y = location.Y;
	movement.z = location.Z;

	FRotator rotation = GetPawn()->GetActorRotation();
	movement.Pitch = rotation.Pitch;
	movement.Yaw = rotation.Yaw;
	movement.Roll = rotation.Roll;

	FVector velocity = GetPawn()->GetVelocity();
	movement.vx = velocity.X;
	movement.vy = velocity.Y;
	movement.vz = velocity.Z;

	char tempBuffer[1024] = { 0 };
	memcpy(tempBuffer, &header, packetHeaderSize);
	memcpy(tempBuffer + packetHeaderSize, &movement, sizeof(Packet::Payload::MOVEMENT));

	Packet::Header::DEFAULT* debugHeader = reinterpret_cast<Packet::Header::DEFAULT*>(tempBuffer);
	Packet::Payload::MOVEMENT* debugMovement = reinterpret_cast<Packet::Payload::MOVEMENT*>(tempBuffer + packetHeaderSize);
	//printf("Send Packet: Header Type=%d, Size=%d, Data ID=%d, X=%f, Y=%f, Z=%f, Pitch=%f, Yaw=%f, Roll=%f, Vx=%f, Vy=%f, Vz=%f\n", debugHeader->type, debugHeader->size, debugMovement->id, debugMovement->x, debugMovement->y, debugMovement->z, debugMovement->Pitch, debugMovement->Yaw, debugMovement->Roll, debugMovement->vx, debugMovement->vy, debugMovement->vz);

	send(*cachedSocket, tempBuffer, packetHeaderSize + sizeof(Packet::Payload::MOVEMENT), 0);
}

void ASHNetPlayerController::UpdateOtherPlayerMovement()
{
	GEngine->AddOnScreenDebugMessage(4, 0.5f, FColor::Emerald, FString::Printf(TEXT("movementWorks Size = %d"), movementWorks.size()));

    //std::lock_guard<std::mutex> lock(movementMutex);
	while (!movementWorks.empty())
	{
		Packet::Payload::MOVEMENT movementData = movementWorks.front();
		movementWorks.pop();


		GEngine->AddOnScreenDebugMessage(5, 30.f, FColor::Blue, FString::Printf(TEXT("Update Data = id=%d, index=%d, x=%f, y=%f, z=%f, Pitch=%f, Yaw=%f, Roll=%f, vx=%f, vy=%f, vz=%f"), movementData.id, movementData.index, movementData.x, movementData.y, movementData.z, movementData.Pitch, movementData.Yaw, movementData.Roll, movementData.vx, movementData.vy, movementData.vz));
		//printf("Update Data = id=%d, x=%f, y=%f, z=%f, Pitch=%f, Yaw=%f, Roll=%f, vx=%f, vy=%f, vz=%f\n", movementData.id, movementData.x, movementData.y, movementData.z, movementData.Pitch, movementData.Yaw, movementData.Roll, movementData.vx, movementData.vy, movementData.vz);

		ACharacter* otherPlayer = otherPlayers[movementData.id];
		if (otherPlayer == nullptr)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			otherPlayer = GetWorld()->SpawnActor<ACharacter>(otherCharacters[movementData.index], FVector(movementData.x, movementData.y, movementData.z), FRotator(movementData.Pitch, movementData.Yaw, movementData.Roll), spawnParams);
			otherPlayers[movementData.id] = otherPlayer;

			GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, TEXT("Character Spawned"));
		}

		otherPlayer->SetActorLocation(FVector(movementData.x, movementData.y, movementData.z));
		otherPlayer->SetActorRotation(FRotator(movementData.Pitch, movementData.Yaw, movementData.Roll));
		otherPlayer->GetCharacterMovement()->Velocity = FVector(movementData.vx, movementData.vy, movementData.vz);
	}
}

void ASHNetPlayerController::SendVoteData(int id_)
{
    header.type = 3;
    header.size = packetHeaderSize + sizeof(Packet::Payload::VOTE_DATA);

    Packet::Payload::VOTE_DATA voteData;
    voteData.id = cachedID;
    voteData.idTarget = id_;

    char tempBuffer[64] = { 0 };
    memcpy(tempBuffer, &header, packetHeaderSize);
    memcpy(tempBuffer + packetHeaderSize, &voteData, sizeof(Packet::Payload::VOTE_DATA));

    Packet::Header::DEFAULT* debugHeader = reinterpret_cast<Packet::Header::DEFAULT*>(tempBuffer);
    Packet::Payload::VOTE_DATA* debugVoteData = reinterpret_cast<Packet::Payload::VOTE_DATA*>(tempBuffer + packetHeaderSize);
    //printf("Send Packet: Header Type=%d, Size=%d, Data VoteID=%d\n", debugHeader->type, debugHeader->size, debugVoteData->idTarget);
    GEngine->AddOnScreenDebugMessage(10, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, Data ID=%d, VoteID=%d\n"), debugHeader->type, debugHeader->size, debugVoteData->id, debugVoteData->idTarget));
    
    send(*cachedSocket, tempBuffer, packetHeaderSize + sizeof(Packet::Payload::VOTE_DATA), 0);
}

void ASHNetPlayerController::KilledPlayer()
{
    //UMeshComponent* mesh = otherPlayers[id_]->FindComponentByClass<UMeshComponent>();
    //if (mesh)
    //{
    //    mesh->SetVisibility(false);
    //}

    while (!killWorks.empty())
    {
        int data = killWorks.front();
        killWorks.pop();
         
        if (data >= 0 && data <= 4)
        {
            otherPlayers[data]->SetActorHiddenInGame(true);
        }
    }
}

void ASHNetPlayerController::SendMissionClear(int missionId_)
{
    header.type = 5;
    header.size = packetHeaderSize + sizeof(Packet::Payload::MISSION_PROCESS_DATA);

    Packet::Payload::MISSION_PROCESS_DATA missionProcessData;
    missionProcessData.id = cachedID;
    missionProcessData.missionId = missionId_;

    char tempBuffer[64] = { 0 };
    memcpy(tempBuffer, &header, packetHeaderSize);
    memcpy(tempBuffer + packetHeaderSize, &missionProcessData, sizeof(Packet::Payload::MISSION_PROCESS_DATA));

    Packet::Header::DEFAULT* debugHeader = reinterpret_cast<Packet::Header::DEFAULT*>(tempBuffer);
    Packet::Payload::MISSION_PROCESS_DATA* debugMissionProcessData = reinterpret_cast<Packet::Payload::MISSION_PROCESS_DATA*>(tempBuffer + packetHeaderSize);
    //printf("Send Packet: Header Type=%d, Size=%d, Data VoteID=%d\n", debugHeader->type, debugHeader->size, debugVoteData->idTarget);
    GEngine->AddOnScreenDebugMessage(21, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, Data ID=%d, MissionID=%d\n"), debugHeader->type, debugHeader->size, debugMissionProcessData->id, debugMissionProcessData->missionId));
}

void ASHNetPlayerController::SendKilledPlayerId(int idKilled_)
{
    header.type = 4;
    header.size = packetHeaderSize + sizeof(Packet::Payload::KILL);

    Packet::Payload::KILL imposterKillData;
    imposterKillData.idKill = idKilled_;

    char tempBuffer[64] = { 0 };
    memcpy(tempBuffer, &header, packetHeaderSize);
    memcpy(tempBuffer + packetHeaderSize, &imposterKillData, sizeof(Packet::Payload::KILL));

    Packet::Header::DEFAULT* debugHeader = reinterpret_cast<Packet::Header::DEFAULT*>(tempBuffer);
    Packet::Payload::KILL* debugKill = reinterpret_cast<Packet::Payload::KILL*>(tempBuffer + packetHeaderSize);
    //printf("Send Packet: Header Type=%d, Size=%d, Data VoteID=%d\n", debugHeader->type, debugHeader->size, debugVoteData->idTarget);
    GEngine->AddOnScreenDebugMessage(22, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, KilledID=%d\n"), debugHeader->type, debugHeader->size, debugKill->idKill));
}
