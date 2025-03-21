// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission.h"
#include "Components/ShapeComponent.h"

// Sets default values
AMission::AMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UShapeComponent>(TEXT("OverlapComp"));
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
	if (OtherActor.IsA<APlayer>()) {
		auto pl = Cast<APlayer>(OtherActor);

		// 1. �÷��̾� �ӹ� ���� Ȯ��

		// 2. �ӹ� ������ ���� �÷��̾��� ��� return;
		if (�ش� �÷��̾����� �ӹ� ������ ����) {
			activePlayerId = -1;
			return;
		}
		// �ӹ� ������ �ִ� �÷��̾��� ���
		activePlayerId = pl->playerId; // Ư�� �÷��̾� �ĺ���
		activePlayer = pl;

		// 3. �ش� �÷��̾��� ��Ʈ�ѷ� �ҷ����� (3�� �׸��� VR ���������� �����ɵ�)
		auto controller = pl->GetController(); // �̰ų� �ƴϸ� �׳� ->Controller �� ��·�� �� �÷��̾��� ��Ʈ�ѷ�
		if (controller == nullptr) return;

		// 3-1. �ش� �÷��̾��� ���콺 Ŀ�� ���̱�
		controller->bShowMouseCursor = true;

		// 4. �÷��̾ �ӹ� ������ ���� ǥ��
		bMissionProcessing = true;

		// ���� �۾��� ���� ���� ��� true �� ��ȯ�ϰ�, true �� ��ȯ���� �̼� ���ʹ� ���� ���� ���� �۾��� �Ѵ�
		return true;
	}
	*/

	return false;
}

bool AMission::OverlapEventEnd(AActor* OtherActor)
{
	if (!bMissionProcessing) return false;

	/*
	if (OtherActor.IsA<APlayer>()) {
		auto pl = Cast<APlayer>(OtherActor);

		// �ӹ� �������� �÷��̾ �ƴϸ� ��� ����
		if (pl->playerId != activePlayerId) return;

		// 1. �ӹ����� �÷��̾� �ĺ��� �ʱ�ȭ
		activePlayerId = -1;
		activePlayer = nullptr;

		// 2. �ӹ� ����
		bMissionProcessing = false;

		// ���� �۾��� ���� ���� ��� true �� ��ȯ�ϰ�, true �� ��ȯ���� �̼� ���ʹ� �ʱ�ȭ ���� �۾��� �Ѵ�. (�̼��� ���������� �Ϸ� ���� ��츸 �̼� ���൵ �ø��� ���� ����!)
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
	// �÷��̾� ī�޶��� ���� ���� �� ��ġ ����
	FVector cameraDir = this->GetActorLocation() - activePlayer->ī�޶�->GetActorLocation();
	FRotator cameraRot = cameraDir.Rotation();
	activePlayer->ī�޶�->SetRelativeRotation(cameraRot);
	*/
}

