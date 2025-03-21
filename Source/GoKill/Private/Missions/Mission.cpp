// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/Mission.h"
#include "Components/SphereComponent.h"
#include "Characters/GK_Player.h"
#include "Components/SceneComponent.h"

// Sets default values
AMission::AMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    SetRootComponent(RootComp);

	// overlap, mesh, ui 사이즈 위치는 블루프린트에서 조정해보고 설정
	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(RootComponent);

	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AMission::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMission::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMission::OverlapEventBegin(AActor* OtherActor)
{
	if (bMissionProcessing) return false;

	/*
	if (OtherActor.IsA<AGK_Player>()) {
		auto pl = Cast<AGK_Player>(OtherActor);

		// 1. 플레이어 임무 권한 확인

		// 2. 임무 권한이 없는 플레이어일 경우 return;
		if (해당 플레이어한테 임무 권한이 없음) {
			activePlayerId = -1;
			return;
		}
		// 임무 권한이 있는 플레이어일 경우
		activePlayerId = pl->playerId; // 특정 플레이어 식별자
		activePlayer = pl;

		// 4. 플레이어가 임무 수행중 여부 표시
		bMissionProcessing = true;

		// 공통 작업이 전부 끝날 경우 true 를 반환하고, true 를 반환받은 미션 액터는 랜덤 세팅 등의 작업을 한다
		return true;
	}
	*/

	return false;
}

bool AMission::OverlapEventEnd(AActor* OtherActor)
{
	if (!bMissionProcessing) return false;

	/*
	if (OtherActor.IsA<AGK_Player>()) {
		auto pl = Cast<AGK_Player>(OtherActor);

		// 임무 진행중인 플레이어가 아니면 상관 없음
		if (pl->playerId != activePlayerId) return;

		// 1. 임무중인 플레이어 식별자 초기화
		activePlayerId = -1;
		activePlayer = nullptr;

		// 2. 임무 종료
		bMissionProcessing = false;

		// 공통 작업이 전부 끝날 경우 true 를 반환하고, true 를 반환받은 미션 액터는 초기화 등의 작업을 한다. (미션이 성공적으로 완료 됐을 경우만 미션 진행도 올리기 잊지 말기!)
		return true;
	}
	*/

	return false;
}

bool AMission::PlayerAfterDie(int32 playerId)
{
	if (playerId == activePlayerId) {
		bMissionProcessing = false;
		activePlayerId = -1;
		activePlayer = nullptr;

		return true;
	}
	return false;
}

void AMission::MissionFocusOn()
{
	/*
	// 플레이어 카메라의 각도 변경 및 위치 변경
	FVector cameraDir = this->GetActorLocation() - activePlayer->카메라->GetActorLocation();
	FRotator cameraRot = cameraDir.Rotation();
	activePlayer->카메라->SetRelativeRotation(cameraRot);
	*/
}