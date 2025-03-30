// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/Scan.h"
#include "Components/SphereComponent.h"
#include "Characters/GK_Player.h"
#include "shDebug.h"
#include "Missions/MissionHandler.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScan::AScan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
    if (TempMesh.Succeeded()) {
        MeshComp->SetStaticMesh(TempMesh.Object);

    }

    MissionId = 1;
    MissionName = "Scan";
}

// Called when the game starts or when spawned
void AScan::BeginPlay()
{
	Super::BeginPlay();
    
    OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AScan::OnMissionOverlap);
    OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AScan::OnMissionEndOverlap);
}

// Called every frame
void AScan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(activePlayer == nullptr) {
        return;
    }

    CurrentTime += DeltaTime;

    // 스캔하는 동안 계속 플레이어가 있는가
    if (StartPlayerId != activePlayerId) {
        Print("Player Change", FColor::Red);
        OverlapEventEnd(activePlayer);
        StartPlayerId = -1;
        CurrentTime = 0.0f;
        return;
    }

    if (CurrentTime >= ScanTime) {
        Print("Scan : End", FColor::Green);

        // 스캔한 플레이어 정체 출력
        //FString logMsg = UEnum::GetValueAsString(OverlapComp->GetCollisionEnabled());
        //FString logMsg = UEnum::GetValueAsString(activePlayer->GetState()); // UEnum 이 아니라 그냥 enum 이어서 안 됨

        //Print(logMsg, FColor::Blue);

        if (activePlayer->GetState() == CharacterState::CrewMember) {
            Print("CrewMember", FColor::Blue);
        }
        else if (activePlayer->GetState() == CharacterState::Imposter) {
            Print("Imposter", FColor::Blue);
        }
        else {
            Print("UNKNOWN", FColor::Blue);
        }

        mHandler->MissionClear();
        OverlapEventEnd(activePlayer);
        StartPlayerId = -1;
        CurrentTime = 0.0f;
    } 
}

void AScan::OnMissionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    bool missionStart = OverlapEventBegin(OtherActor);

    if (missionStart) {
        StartPlayerId = activePlayerId;

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScanSound, GetActorLocation(), 1.0f, 1.0f, 2.0f, Attenuation, Concurrency);
        Print("Scan : Start", FColor::Green);
    }
}

void AScan::OnMissionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bool missionEnd = OverlapEventEnd(OtherActor);

    if (missionEnd) {
        StartPlayerId = -1;
        CurrentTime = 0.0f;
    }
}

