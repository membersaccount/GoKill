// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/UnlockManifolds.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/GK_Player.h"
#include "shDebug.h"

AUnlockManifolds::AUnlockManifolds()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);

	}
}

void AUnlockManifolds::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AUnlockManifolds::OnMissionOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AUnlockManifolds::OnMissionEndOverlap);
}

void AUnlockManifolds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnlockManifolds::OnMissionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool missionStart = OverlapEventBegin(OtherActor);

    if (missionStart) {
	    MissionFocusOn();

        // 마우스 버전
        auto pc = Cast<APlayerController>(activePlayer->GetController());
        pc->bShowMouseCursor = true;

        // 랜덤 숫자 뿌리기
    }
}

void AUnlockManifolds::OnMissionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bool missionEnd = OverlapEventEnd(OtherActor);

    if (missionEnd) {
        // 마우스 버전
        auto pc = Cast<APlayerController>(activePlayer->GetController());
        pc->bShowMouseCursor = false;

        // 미션 성공일 경우 미션 진행도 늘리기

        // 미션 초기화
    }
}