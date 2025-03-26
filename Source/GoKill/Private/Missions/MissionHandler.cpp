// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/MissionHandler.h"
#include "Characters/GK_Player.h"

MissionHandler::MissionHandler()
{
}

MissionHandler::~MissionHandler()
{
}

void MissionHandler::SetMissionListIdx(int idx)
{
    MissionListIdx = idx;
}

void MissionHandler::SetPlayer(AGK_Player* player)
{
    activePlayer = player;
}

void MissionHandler::MissionClear()
{
    if (MissionListIdx == -1 || activePlayer == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("No correct information about player who clear mission."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("%d player clear mission idx %d"), activePlayer->PlayerId, MissionListIdx);

    // 임포스터가 아닐 경우 총 미션 성공도 올리기
    if (activePlayer->GetState() != CharacterState::Imposter) {
        // 서버 : 총 미션 성공도 증가
    }

    // 플레이어의 미션 임무 completed 처리
    activePlayer->MissionCompleted[MissionListIdx] = true;

    MissionListIdx = -1;
}
