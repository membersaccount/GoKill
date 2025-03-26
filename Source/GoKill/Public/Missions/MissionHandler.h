// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GOKILL_API MissionHandler
{
private:
    // 존재하는 미션 아이디 목록 (확률 조정을 위해 중복 포함)
    TArray<int> MissionIds = {1, 2};

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
    void MissionHandout(TArray<class AGK_Player*> players, int numOfMissions);
};
