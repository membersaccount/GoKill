// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MissionData.h"

/**
 * 
 */
class GOKILL_API MissionHandler
{
private:
    // 미션 식별자 목록 (+확률 조정)
    //TArray<int32> MissionIds = {1, 2, 2};
    TArray<MissionInfo> MissionDataList;

    int MissionListIdx = -1;
    
    class AGK_Player* activePlayer;

public:
	MissionHandler();
	~MissionHandler();

    void SetMissionListIdx(int idx);

    void SetPlayer(class AGK_Player* player);

    // 미션 완료
    void MissionClear();

    // 미션 랜덤 배부
    void MissionHandoutAll(TArray<class AGK_Player*> players, int numOfMissions);
    void MissionHandout(class AGK_Player* player, int numOfMissions);

private:
    // 미션 리스트 세팅 (해당 레벨에 존재하는 미션들이 등록된다) -> BeginPlay 시점에서 최초에 적용하기
    void SetMissionDataList(UWorld* world);
};
