// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/UnlockManifolds.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Engine/StaticMesh.h"

AUnlockManifolds::AUnlockManifolds()
{
	// overlap, mesh, ui 사이즈 위치는 블루프린트에서 조정해보고 설정
	//OverlapComp = CreateDefaultSubobject<UShapeComponent>(TEXT("OverlapComp"));
	//OverlapComp->SetupAttachment(RootComponent);

	//OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

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

	//OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AUnlockManifolds::OnMissionOverlap);
}

void AUnlockManifolds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnlockManifolds::OnMissionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapEventBegin(OtherActor);
	MissionFocusOn();
}
