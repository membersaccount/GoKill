// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/UnlockManifolds.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/GK_Player.h"
#include "shDebug.h"
#include "Missions/UnlockManifoldsWidget.h"
#include "Components/WidgetComponent.h"
#include "Missions/MissionHandler.h"
#include "Kismet/GameplayStatics.h"

AUnlockManifolds::AUnlockManifolds()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
        MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
        MeshComp->SetRelativeScale3D(FVector(0.02f, 0.5f, 0.25f));
	}

    // UWidgetComponent
    BtnComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BtnWidget"));
    BtnComp->SetupAttachment(MeshComp);

    ConstructorHelpers::FClassFinder<UUnlockManifoldsWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/RGY/Blueprints/Mission/WBP_UnlockManifoldsPad.WBP_UnlockManifoldsPad_C'"));
    if (TempWidget.Succeeded()) {
        BtnComp->SetWidgetClass(TempWidget.Class);
        BtnComp->SetRelativeLocation(FVector(51.0f, -39.0f, -32.0f));
        BtnComp->SetRelativeScale3D(FVector(1.0f, 0.35f, 0.8f));
    }

    MissionId = 2;
    MissionName = "Unlock Manifolds";
}

void AUnlockManifolds::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AUnlockManifolds::OnMissionOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AUnlockManifolds::OnMissionEndOverlap);

    if(BtnComp == nullptr) return;
    BtnWidget = Cast<UUnlockManifoldsWidget>(BtnComp->GetUserWidgetObject());
}

void AUnlockManifolds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


    if(BtnWidget == nullptr) return;
    if (activePlayer != nullptr && bMissionProcessing && BtnWidget->bError) {
        BtnWidget->bError = false;
        // 에러음 발생
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ErrorSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, Attenuation, Concurrency);
    }

    if (activePlayer != nullptr && bMissionProcessing && !bSuccess) {
        if (BtnWidget->bSuccess) {
            bSuccess = true;
            // 성공음 발생
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, Attenuation, Concurrency);

            // 미션 진행도 늘리기
            mHandler->MissionClear();

            // 미션 종료
            bool end = OverlapEventEnd(activePlayer);
            if(end) {
                bWaiting = true;

                GetWorldTimerManager().SetTimer(WaitingTimer, FTimerDelegate::CreateLambda([this]() -> void {
                    BtnWidget->ResetBtn();
                    bWaiting = false;

                    GetWorldTimerManager().ClearTimer(WaitingTimer);
                }), WaitingTime, true);
            }

            Print("Mission Success", FColor::Blue);
        }
    }
}

void AUnlockManifolds::OnMissionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (BtnWidget == nullptr) {
        Print("BtnWidget is nullptr", FColor::Red);
        return;
    }

    if(bWaiting) return;

	bool missionStart = OverlapEventBegin(OtherActor);

    if (missionStart) {
        // 카메라 미션 쪽으로 돌리기 -> 현재 버그 발생 중
	    //MissionFocusOn();

        // 마우스 버전
        if(bUseMouse) {
            auto pc = Cast<APlayerController>(activePlayer->GetController());
            pc->bShowMouseCursor = true;
            //pc->SetInputMode(FInputModeUIOnly());
        }

        // 랜덤 숫자 뿌리기
        BtnWidget->ResetBtn();
    }
}

void AUnlockManifolds::OnMissionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bool missionEnd = OverlapEventEnd(OtherActor);

    if (missionEnd) {
        // 미션 초기화
        if(BtnWidget == nullptr) return;
        BtnWidget->ResetBtn();
    }
}
