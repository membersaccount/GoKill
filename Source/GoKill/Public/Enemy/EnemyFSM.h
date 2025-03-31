// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle UMETA( DisplayName = "대기" ) , // 0
	Move UMETA( DisplayName = "이동" ) , // 1
	Attack UMETA( DisplayName = "공격" ) // 2
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOKILL_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// 대기
	void IdleState();

	// 이동
	void MoveState();

	// 공격
	void AttackState();


	// 일정 시간 기다렸다 이동 상태로 전환
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 1.0f;

	float currentTime = 0.0f;


	// 타겟
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AGK_Player* target;

	// 소유 액터
	UPROPERTY()
	class AEnemy* me;


	// 공격 범위
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 80.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 1.0f;

	// 사용 중인 애니메이션 블루프린트
	UPROPERTY()
	class UEnemyAnim* Anim;

	// Enemy 를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 길찾기 수행시 랜덤 위치
	FVector randomPos;

	// 랜덤 위치 가져오기
	bool GetRandomPositionNavMesh(FVector centerLocation, float radius, FVector& dest);

	UFUNCTION()
	void OnAttackEnd();

    float GetPlayerLocationAngle();

    UFUNCTION()
	void OnLegOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
    UFUNCTION()
	void OnLegOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool bOverlap = false;
    class AGK_Player* OverlapPlayer;
};
