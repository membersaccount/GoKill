// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyFSM.h"
#include "Characters/GK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyAnim.h"
#include "NavigationSystem.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
        // 월드에서 ATPSPlayer 찾아오기
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AGK_Player::StaticClass());
    // Get 으로 되어있는 애들은 가져와서 정상적으로 가져왔는지 체크하는게 좋다.

    // ATPSPlayer 타입으로 캐스팅
    target = Cast<AGK_Player>(actor);

    // 소유 객체 가져오기
    me = Cast<AEnemy>(GetOwner());

    Anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

    // AIController 할당하기
    ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

    switch (mState)
    {
    case EEnemyState::Idle:
        IdleState();
        break;
    case EEnemyState::Move:
        MoveState();
        break;
    case EEnemyState::Attack:
        AttackState();
        break;
    default:
        break;
    }
}

void UEnemyFSM::IdleState()
{
    // 시간 진행
    currentTime += GetWorld()->DeltaTimeSeconds;

    // 대기 시간 초과시 이동상태로 전환
    if (currentTime > idleDelayTime) {
        mState = EEnemyState::Move;

        // 애니메이션 상태 동기화
        Anim->animState = mState;

        // 경과 시간 초기화
        currentTime = 0.0f;

        GetRandomPositionNavMesh(me->GetActorLocation(), 500.0f, randomPos);

    }
}

void UEnemyFSM::MoveState()
{
    // 타겟 목적지
    FVector destination = target->GetActorLocation();

    // 방향
    FVector dir = destination - me->GetActorLocation();

    auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

    // 목적지 길찾기 경로 데이터 검색
    FPathFindingQuery query;
    FAIMoveRequest req;

    // 목적지에서 인지할 수 있는 범위
    req.SetAcceptanceRadius(3);
    req.SetGoalLocation(destination);

    // 길 찾기를 위한 쿼리 생성
    ai->BuildPathfindingQuery(req, query);

    // 길 찾기 결과 가져오기
    FPathFindingResult r = ns->FindPathSync(query);

    // 목적지까지 길 찾기 성공 여부 확인
    if (r.Result == ENavigationQueryResult::Success) {
        // 타겟으로 이동
        ai->MoveToLocation(destination);
    }
    else {
        // 랜덤 위치로 이동
        auto result = ai->MoveToLocation(randomPos);

        // 목적지에 도착하면
        if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
            // 새로운 랜덤 위치 가져오기
            GetRandomPositionNavMesh(me->GetActorLocation(), 500.0f, randomPos);
        }
    }

    // 타겟과 거리가 attackRange 이내면 && 타겟이 전방에 존재하면 공격상태로 전환
    if (dir.Size() <= attackRange && GetPlayerLocationAngle() < 90) {
        ai->StopMovement();

        mState = EEnemyState::Attack;
        Anim->animState = mState;
        Anim->bAttackPlay = true;
    }
}

void UEnemyFSM::AttackState()
{
    // 공격 범위 내에서 일정 시간마다 공격
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime >= attackDelayTime) {
        currentTime = 0.0f;
        Anim->bAttackPlay = true;
    }

    // 타겟과의 거리
    float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

    // 공격 범위를 넘어서면 || 타겟이 전방에 있지 않으면 Move 상태로 전환
    if (distance > attackRange || GetPlayerLocationAngle() >= 90) {
        currentTime = 0.0f;
        mState = EEnemyState::Move;
        Anim->animState = mState;

        GetRandomPositionNavMesh(me->GetActorLocation(), 500.0f, randomPos);
    }
}

bool UEnemyFSM::GetRandomPositionNavMesh(FVector centerLocation, float radius, FVector& dest)
{
    auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

    FNavLocation loc;
    bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
    dest = loc.Location;

    return result;
}

void UEnemyFSM::OnAttackEnd()
{
    Anim->bAttackPlay = false;
}

float UEnemyFSM::GetPlayerLocationAngle()
{
    FVector LookDir = me->GetActorForwardVector();
    FVector PlayerLocDir = target->GetActorLocation() - me->GetActorLocation();

    // 내적을 구하면 절대값(a) * 절대값(b) * cos(사이각(라디안)) 이 된다
    float Dot = FVector::DotProduct(LookDir, PlayerLocDir);
    // 내적을 역 코사인
    float AcosAngle = FMath::Acos(Dot);
    // 라디안을 우리가 쓰는 도수법으로 변경
    float AngleDegree = FMath::RadiansToDegrees(AcosAngle);

    return AngleDegree;
}

