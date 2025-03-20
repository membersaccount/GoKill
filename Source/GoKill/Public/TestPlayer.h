// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestPlayer.generated.h"

UCLASS()
class GOKILL_API ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	public:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* VRCamera;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* IMC_VR;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_VRMove;

	void Move(const struct FInputActionValue& value);
};
