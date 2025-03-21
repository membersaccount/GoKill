// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.generated.h"

UCLASS()
class GOKILL_API AMission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMission();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// 미션 아이템이 사용중인가 여부
	bool bMissionProcessing = false;

	// 미션 아이템을 사용중인 플레이어의 식별자
	int32 activePlayerId = -1;

	class AActor* activePlayer; // 나중에 APlayer 같은걸로 변경
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UShapeComponent* OverlapComp;

public:
	// 미션 범위에 overlap 됐을 때 공통적인 작업 : overlap 된 플레이어가 작업 시작하면 true 반환
	virtual bool OverlapEventBegin(AActor* OtherActor);

	// 미션 범위의 overlap 이 끝났을 때 공통적인 작업 : overlap 끝난 플레이어가 작업 중이던 플레이어면 true 반환
	virtual bool OverlapEventEnd(AActor* OtherActor);

	// 미션 범위 안에서 플레이어가 사망했을 시 미션중이던 플레이어면 미션 취소 작업 : 죽은 플레이어가 작업 중이던 플레이어면 true 반환
	virtual bool PlayerAfterDie(int32 playerId);

	// 플레이어 카메라가 미션 아이템을 주목해야할지는 미션마다 다르기에 따로 분리
	virtual void MissionFocusOn();
};
