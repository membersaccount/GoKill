// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/Mission.h"
#include "Components/SphereComponent.h"
#include "Characters/GK_Player.h"
#include "Components/SceneComponent.h"
#include "shDebug.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	///*
	if (OtherActor->IsA<AGK_Player>()) {
		auto pl = Cast<AGK_Player>(OtherActor);

		// 1. 플레이어 임무 권한 확인

		// 2. 임무 권한이 없는 플레이어일 경우 return;
		//if (해당 플레이어한테 임무 권한이 없음) {
		//	activePlayerId = -1;
		//	return;
		//}
		// 임무 권한이 있는 플레이어일 경우
		activePlayerId = pl->PlayerId; // 특정 플레이어 식별자
		activePlayer = pl;

		// 4. 플레이어가 임무 수행중 여부 표시
		bMissionProcessing = true;

		// 공통 작업이 전부 끝날 경우 true 를 반환하고, true 를 반환받은 미션 액터는 랜덤 세팅 등의 작업을 한다
		return true;
	}
	//*/

	return false;
}

bool AMission::OverlapEventEnd(AActor* OtherActor)
{
	if (!bMissionProcessing) return false;

	///*
	if (OtherActor->IsA<AGK_Player>()) {
		auto pl = Cast<AGK_Player>(OtherActor);

		// 임무 진행중인 플레이어가 아니면 상관 없음
		if (pl->PlayerId != activePlayerId) return false;

        // 마우스 버전
        if(bUseMouse) {
            auto pc = Cast<APlayerController>(activePlayer->GetController());
            pc->bShowMouseCursor = false;
            pc->SetInputMode(FInputModeGameAndUI());
        }

		// 1. 임무중인 플레이어 식별자 초기화
		activePlayerId = -1;
		activePlayer = nullptr;

		// 2. 임무 종료
		bMissionProcessing = false;
        bSuccess = false;

		// 공통 작업이 전부 끝날 경우 true 를 반환하고, true 를 반환받은 미션 액터는 초기화 등의 작업을 한다. (미션이 성공적으로 완료 됐을 경우만 미션 진행도 올리기 잊지 말기!
        return true;
	}
	//*/

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
	// 플레이어 카메라의 각도 변경 및 위치 변경
    FVector MissionDir = FVector((GetActorForwardVector() * -1).X, (GetActorForwardVector() * -1).Y, 0.0f).GetSafeNormal();

    /*
    FVector CameraDir = FVector(activePlayer->GetPlayerCamera()->GetForwardVector().X, activePlayer->GetPlayerCamera()->GetForwardVector().Y, 0.0f).GetSafeNormal();
    FVector SubDir = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f) - FVector(activePlayer->GetPlayerCamera()->GetComponentLocation().X, activePlayer->GetPlayerCamera()->GetComponentLocation().Y, 0.0f);

    // 내적을 구하면 절대값(a) * 절대값(b) * cos(사이각(라디안)) 이 된다
    float Dot = FVector::DotProduct(CameraDir, MissionDir);
    // 내적을 역 코사인
    float AcosAngle = FMath::Acos(Dot);
    // 라디안을 우리가 쓰는 도수법으로 변경
    float AngleDegree = FMath::RadiansToDegrees(AcosAngle);

    // 사이각은 구했는데 이게 카메라 바라보는 기준으로 오른편에 있는지 왼편에 있는지 어떻게..
    activePlayer->AddControllerYawInput(AngleDegree);

    Print(FString::FormatAsNumber(AngleDegree), FColor::Red);

    //Print(GetActorForwardVector().ToString(), FColor::Blue);
    //Print(cameraDir.ToString(), FColor::Red);
    //Print(cameraRot.ToString(), FColor::Green);
    */

    activePlayer->SetActorRotation(MissionDir.Rotation());
    auto pc = Cast<APlayerController>(activePlayer->GetController());
    pc->SetControlRotation(MissionDir.Rotation());
}

int32 AMission::GetMissionId()
{
    return MissionId;
}

bool AMission::GetIsSuccess()
{
    return bSuccess;
}
