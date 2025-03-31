#include "Characters/GK_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

AGK_CharacterBase::AGK_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGK_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGK_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGK_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

CharacterState AGK_CharacterBase::GetState()
{
    return State;
}

void AGK_CharacterBase::SetState(CharacterState state_)
{
    State = state_;
}

CharacterMission AGK_CharacterBase::GetMission()
{
    return MissionProgress;
}

void AGK_CharacterBase::SetMission(CharacterMission mission_)
{
    MissionProgress = mission_;
}

void AGK_CharacterBase::GameOver()
{
    bDie = true;
    GetCharacterMovement()->GravityScale = 0.0f;
    SetActorLocation(FVector(-1200, 20, 500));
    SetActorRotation(FRotator::ZeroRotator);
}

