#include "Characters/GK_CharacterBase.h"

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

