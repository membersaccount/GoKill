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


public: // Vote
    void SendVoteData(int id_);
    void KilledPlayer();

public: // Mission
    void SendMissionClear(int missionId_);

public: // Imposter Kill
    void SendKilledPlayerId(int idKilled_);

public:
	std::mutex movementMutex;
	std::queue<Packet::Payload::MOVEMENT> movementWorks;
	int cachedID;
    int baseIndex;
    std::queue<int> killWorks;
    std::atomic<int> updateIndex = -1;
    std::atomic<bool> isGameStart = false;
    std::atomic<bool> isImposter = false;

private:
	std::unordered_map<int, ACharacter*> otherPlayers;


	SOCKET* cachedSocket;
	SHNetCore* cachedNetwork;

	UPROPERTY()
	TSubclassOf<ACharacter> otherCharacter;
    TArray<TSubclassOf<ACharacter>> otherCharacters;

	Packet::Header::DEFAULT header;
	Packet::Payload::MOVEMENT movement;
    int packetHeaderSize = sizeof(Packet::Header::DEFAULT);
};
