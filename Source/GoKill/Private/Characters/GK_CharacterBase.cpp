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

