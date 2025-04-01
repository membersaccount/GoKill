#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/GKEnums.h"
#include "Data/MissionData.h"
#include "GK_CharacterBase.generated.h"

UCLASS()
class GOKILL_API AGK_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGK_CharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
    class USoundWave* ImposterWinSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
    class USoundWave* CrewWinSound;

    bool bDie = false;
    bool bWin = false;

    int PlayerId = -1;

    TArray<MissionInfo> MissionList;

public:
    CharacterState GetState();
    void SetState(CharacterState state_);
    CharacterMission GetMission();
    void SetMission(CharacterMission mission_);

    void GameOver();
    void GameWin();

protected:
	CharacterState State = CharacterState::CrewMember;
	CharacterMission MissionProgress;
};
