// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GOKILL_API MissionHandler
{
private:
    // 미션 식별자 목록 (+확률 조정)
    TArray<int32> MissionIds {1, 2, 2};

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
};
