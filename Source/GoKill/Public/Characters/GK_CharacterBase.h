#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/GKEnums.h"
#include "GK_CharacterBase.generated.h"

UCLASS()
class GOKILL_API AGK_CharacterBase : public ACharacter
{
	GENERATED_BODY()

    //struct MissionInfo
    //{
    //    int32 MissionId;
    //    FString MissionName;
    //    bool Completed;
    //};

public:
	AGK_CharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    int PlayerId = -1;
    //TArray<MissionInfo> MissionList;
    TArray<int32> MissionList;
    TArray<bool> MissionCompleted;
    CharacterState GetState();
    void SetState(CharacterState state_);
    CharacterMission GetMission();
    void SetMission(CharacterMission mission_);

protected:
	CharacterState State;
	CharacterMission MissionProgress;
};
