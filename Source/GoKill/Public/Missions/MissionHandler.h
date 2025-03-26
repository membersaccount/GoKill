// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GOKILL_API MissionHandler
{
private:
    int MissionListIdx = -1;
    class AGK_Player* activePlayer;

public:
	MissionHandler();
	~MissionHandler();

    void SetMissionListIdx(int idx);

    void SetPlayer(class AGK_Player* player);

    // 미션 완료 로직
    void MissionClear();
};
