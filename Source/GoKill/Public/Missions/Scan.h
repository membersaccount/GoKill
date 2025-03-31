// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.h"
#include "Scan.generated.h"

UCLASS()
class GOKILL_API AScan : public AMission
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    // 스캔 타임
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess=true))
	float ScanTime = 3.0f;

    float CurrentTime = 0.0f;

	// 타이머
	FTimerHandle ScanTimer;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    class USoundWave* ScanSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    class USoundAttenuation* Attenuation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    class USoundConcurrency* Concurrency;

public:
    int StartPlayerId = -1;

    UFUNCTION()
	void OnMissionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void OnMissionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
