#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/GKEnums.h"
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

protected:
	CharacterState State;
	CharacterMission MissionProgress;
};
