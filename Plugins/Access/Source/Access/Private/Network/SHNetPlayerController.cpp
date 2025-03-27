#include "Network/SHNetPlayerController.h"
#include "Network/SHNetGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ASHNetPlayerController::ASHNetPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterClass(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (OtherCharacterClass.Succeeded())
	{
		otherCharacter = OtherCharacterClass.Class;
	}
}

void ASHNetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//cachedSocket = GetWorld()->GetGameInstance<USHNetGameInstance>()->cachedSocket;
	cachedNetwork = GetWorld()->GetGameInstance<USHNetGameInstance>()->network;
	cachedNetwork->Run();
	cachedSocket = GetWorld()->GetGameInstance<USHNetGameInstance>()->network->GetSocket();
	cachedNetwork->SetGameInstance(GetWorld()->GetGameInstance<USHNetGameInstance>());
	cachedID = GetWorld()->GetGameInstance<USHNetGameInstance>()->clientID;
	cachedNetwork->SetController(this);
}

void ASHNetPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (cachedID < 1)
	{
		cachedID = GetWorld()->GetGameInstance<USHNetGameInstance>()->clientID;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("ID is -1"));
		return;
	}

	SendSelfMovement();
	UpdateOtherPlayerMovement();

	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString::Printf(TEXT("Socket: %d, SelfID: %d"), static_cast<int>(*cachedSocket), cachedID));
	//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Cyan, FString::Printf(TEXT("OtherCharacters: %d"), otherPlayers.size()));
}

void ASHNetPlayerController::Destroyed()
{
	Super::Destroyed();

	cachedNetwork->DestroyThread();
	closesocket(*cachedSocket);
	WSACleanup();
	//FreeConsole();
}

void ASHNetPlayerController::SendSelfMovement()
{
	header.type = 2;
	header.size = sizeof(Packet::Header::DEFAULT) + sizeof(Packet::Payload::MOVEMENT);
	movement.id = cachedID;

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
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Emerald, FString::Printf(TEXT("movementWorks Size = %d"), movementWorks.size()));

	std::lock_guard<std::mutex> lock(movementMutex);
	while (!movementWorks.empty())
	{
		Packet::Payload::MOVEMENT movementData = movementWorks.front();
		movementWorks.pop();

		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("Update Data = id=%d, x=%f, y=%f, z=%f, Pitch=%f, Yaw=%f, Roll=%f, vx=%f, vy=%f, vz=%f"), movementData.id, movementData.x, movementData.y, movementData.z, movementData.Pitch, movementData.Yaw, movementData.Roll, movementData.vx, movementData.vy, movementData.vz));
		//printf("Update Data = id=%d, x=%f, y=%f, z=%f, Pitch=%f, Yaw=%f, Roll=%f, vx=%f, vy=%f, vz=%f\n", movementData.id, movementData.x, movementData.y, movementData.z, movementData.Pitch, movementData.Yaw, movementData.Roll, movementData.vx, movementData.vy, movementData.vz);

		ACharacter* otherPlayer = otherPlayers[movementData.id];
		if (otherPlayer == nullptr)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			otherPlayer = GetWorld()->SpawnActor<ACharacter>(otherCharacter, FVector(movementData.x, movementData.y, movementData.z), FRotator(movementData.Pitch, movementData.Yaw, movementData.Roll), spawnParams);
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
    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, Data ID=%d, VoteID=%d\n"), debugHeader->type, debugHeader->size, debugVoteData->id, debugVoteData->idTarget));
    
    send(*cachedSocket, tempBuffer, packetHeaderSize + sizeof(Packet::Payload::VOTE_DATA), 0);
}

void ASHNetPlayerController::ApplyVoteResult()
{

}

void ASHNetPlayerController::KickPlayer()
{

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
    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, Data ID=%d, MissionID=%d\n"), debugHeader->type, debugHeader->size, debugMissionProcessData->id, debugMissionProcessData->missionId));
}

void ASHNetPlayerController::SendKilledPlayerId(int idKilled_)
{
    header.type = 5;
    header.size = packetHeaderSize + sizeof(Packet::Payload::IMPOSTER_KILL);

    Packet::Payload::IMPOSTER_KILL imposterKillData;
    imposterKillData.id = cachedID;
    imposterKillData.idKilled = idKilled_;

    char tempBuffer[64] = { 0 };
    memcpy(tempBuffer, &header, packetHeaderSize);
    memcpy(tempBuffer + packetHeaderSize, &imposterKillData, sizeof(Packet::Payload::IMPOSTER_KILL));

    Packet::Header::DEFAULT* debugHeader = reinterpret_cast<Packet::Header::DEFAULT*>(tempBuffer);
    Packet::Payload::IMPOSTER_KILL* debugImposterKill = reinterpret_cast<Packet::Payload::IMPOSTER_KILL*>(tempBuffer + packetHeaderSize);
    //printf("Send Packet: Header Type=%d, Size=%d, Data VoteID=%d\n", debugHeader->type, debugHeader->size, debugVoteData->idTarget);
    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("Send Packet: Header Type=%d, Size=%d, Data ID=%d, MissionID=%d\n"), debugHeader->type, debugHeader->size, debugImposterKill->id, debugImposterKill->idKilled));
}
