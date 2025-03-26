// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/MissionHandler.h"
#include "Characters/GK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "shDebug.h"

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

void MissionHandler::MissionHandoutAll(TArray<AGK_Player*> players, int numOfMissions)
{
    // 무작위 시드 설정
    float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
    int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);
    FMath::RandInit(Seed);

    int MissionCount = MissionIds.Num();

    // 플레이어들한테 numOfMissions 개 만큼 미션을 랜덤 배정
    for (AGK_Player* pl : players) {
        for (int i = 0; i < numOfMissions; i++) {
            int randIdx = FMath::RandRange(0, MissionCount - 1);
            pl->MissionList.Add(MissionIds[randIdx]);
            pl->MissionCompleted.Add(false);
        }
    }
}

void MissionHandler::MissionHandout(class AGK_Player* player, int numOfMissions)
{
    // 무작위 시드 설정
    float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
    int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);
    FMath::RandInit(Seed);

    int MissionCount = MissionIds.Num();

    for (int i = 0; i < numOfMissions; i++) {
        int randIdx = FMath::RandRange(0, MissionCount - 1);
        player->MissionList.Add(MissionIds[randIdx]);
        player->MissionCompleted.Add(false);
    }
}
