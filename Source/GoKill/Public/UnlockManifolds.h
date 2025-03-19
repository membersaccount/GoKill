// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mission.h"
#include "UnlockManifolds.generated.h"

/**
 * 
 */
UCLASS()
class GOKILL_API AUnlockManifolds : public AMission
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AUnlockManifolds();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UShapeComponent* OverlapComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CamComp;
};
