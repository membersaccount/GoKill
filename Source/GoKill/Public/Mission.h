// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.generated.h"

UCLASS()
class GOKILL_API AMission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMission();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// �̼� �������� ������ΰ� ����
	bool bMissionProcessing = false;

	// �̼� �������� ������� �÷��̾��� �ĺ���
	int32 activePlayerId = -1;

	class AActor* activePlayer; // ���߿� APlayer �����ɷ� ����
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UShapeComponent* OverlapComp;

public:
	// �̼� ������ overlap ���� �� �������� �۾� : overlap �� �÷��̾ �۾� �����ϸ� true ��ȯ
	virtual bool OverlapEventBegin(AActor* OtherActor);

	// �̼� ������ overlap �� ������ �� �������� �۾� : overlap ���� �÷��̾ �۾� ���̴� �÷��̾�� true ��ȯ
	virtual bool OverlapEventEnd(AActor* OtherActor);

	// �̼� ���� �ȿ��� �÷��̾ ������� �� �̼����̴� �÷��̾�� �̼� ��� �۾� : ���� �÷��̾ �۾� ���̴� �÷��̾�� true ��ȯ
	virtual bool PlayerAfterDie(int32 playerId);

	// �÷��̾� ī�޶� �̼� �������� �ָ��ؾ������� �̼Ǹ��� �ٸ��⿡ ���� �и�
	virtual void MissionFocusOn();
};
