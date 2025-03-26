#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "shCore.h"

#include <unordered_map>
#include <queue>

#include <mutex>

#include "SHNetPlayerController.generated.h"


class SHNetCore;

UCLASS()
class ACCESS_API ASHNetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASHNetPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	void SendSelfMovement();
	void UpdateOtherPlayerMovement();

public:
	std::mutex movementMutex;
	std::queue<Packet::Payload::MOVEMENT> movementWorks;
	int cachedID;

private:
	std::unordered_map<int, ACharacter*> otherPlayers;


	SOCKET* cachedSocket;
	SHNetCore* cachedNetwork;

	UPROPERTY()
	TSubclassOf<ACharacter> otherCharacter;

	Packet::Header::DEFAULT header;
	Packet::Payload::MOVEMENT movement;

	//bool SpawnTest = true;
};
