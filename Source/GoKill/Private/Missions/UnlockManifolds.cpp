// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/UnlockManifolds.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/GK_Player.h"
#include "shDebug.h"
#include "UnlockManifoldsWidget.h"
#include "Components/WidgetComponent.h"

AUnlockManifolds::AUnlockManifolds()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);

	}

    // UWidgetComponent
    BtnComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BtnWidget"));
    BtnComp->SetupAttachment(MeshComp);

    ConstructorHelpers::FClassFinder<UUnlockManifoldsWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/RGY/Blueprints/Mission/WBP_UnlockManifoldsPad.WBP_UnlockManifoldsPad_C'"));
    if (TempWidget.Succeeded()) {
        BtnComp->SetWidgetClass(TempWidget.Class);
        BtnComp->SetRelativeLocation(FVector(51.0f, -39.0f, -107.0f));
        BtnComp->SetRelativeScale3D(FVector(1.0f, 0.35f, 0.8f));

        BtnWidget = Cast<UUnlockManifoldsWidget>(BtnComp->GetUserWidgetObject());
    }

    MissionId = 1;
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


    if(BtnWidget == nullptr) return;
    if (activePlayer != nullptr && !bSuccess) {
        if (BtnWidget->bSuccess) {
            bSuccess = true;
            // 미션 진행도 늘리기

            // 미션 종료
            OverlapEventEnd(activePlayer);

            Print("Mission Success", FColor::Blue);
        }
    }
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
        BtnWidget->ResetBtn();
    }
}

void AUnlockManifolds::OnMissionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bool missionEnd = OverlapEventEnd(OtherActor);

    if (missionEnd) {
        OverlapEventEnd(OtherActor);
        // 미션 초기화
    }
}
