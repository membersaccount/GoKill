// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyFSM.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Mesh 에 마네킹을 로드
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

    // 파일 읽기 성공했을 경우
    if (TempMesh.Succeeded()) {
        // 로드한 메시를 넣어준다.
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        // 위치값과 회전값 적용 (메시만 이동 / 회전)
        GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
        GetMesh()->SetRelativeScale3D(FVector(0.8f));
    }

    // 애니메이션 블루프린트 할당하기
    ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/RGY/Blueprints/ABP_Enemy.ABP_Enemy_C'"));

    if (TempAnim.Succeeded()) {
        GetMesh()->SetAnimInstanceClass(TempAnim.Class);
    }

    RightLegCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLegCollision"));
    RightLegCollision->SetupAttachment(GetMesh());

    RightLegCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RightLegCollision->SetCollisionObjectType(ECC_Pawn);
    RightLegCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    RightLegCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // EnemyFSM 컴포넌트 추가
    fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

    // 월드에 배치되거나 스폰될 때 자동으로
    // AIController 로부터 Possess 될 수 있도록 설정
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

