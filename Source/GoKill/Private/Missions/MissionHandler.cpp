// Fill out your copyright notice in the Description page of Project Settings.


#include "Missions/MissionHandler.h"
#include "Characters/GK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "shDebug.h"
#include "Missions/Mission.h"

MissionHandler::MissionHandler()
{
    ///*
    // 임시로 넣어주기
    MissionInfo scan, manifolds;

    scan.Id = 1;
    scan.Name = "Scan";

    manifolds.Id = 2;
    manifolds.Name = "Unlock Manifolds";

    MissionDataList = { scan, manifolds, manifolds };
    //*/
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
    activePlayer->MissionList[MissionListIdx].Completed = true;

    MissionListIdx = -1;
}

void MissionHandler::MissionHandoutAll(TArray<AGK_Player*> players, int numOfMissions)
{
    // 무작위 시드 설정
    float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
    int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);
    FMath::RandInit(Seed);

    int MissionCount = MissionDataList.Num();

    // 플레이어들한테 numOfMissions 개 만큼 미션을 랜덤 배정
    for (AGK_Player* pl : players) {
        for (int i = 0; i < numOfMissions; i++) {
            int randIdx = FMath::RandRange(0, MissionCount - 1);
            pl->MissionList.Add(MissionDataList[randIdx]);
        }
    }
}

void MissionHandler::MissionHandout(class AGK_Player* player, int numOfMissions)
{
    // 무작위 시드 설정
    float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
    int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);
    FMath::RandInit(Seed);

    int MissionCount = MissionDataList.Num();

    for (int i = 0; i < numOfMissions; i++) {
        int randIdx = FMath::RandRange(0, MissionCount - 1);
        player->MissionList.Add(MissionDataList[randIdx]);
    }
}

void MissionHandler::SetMissionDataList(UWorld* world)
{
    if(world == nullptr) return;
    /*
    // 현재 맵의 액터들 중 Mission 클래스를 상속하고 있는 액터들을 MissionDataList 에 넣어준다
    // 불완전한 형식 오류 발생 중
    for (TActorIterator<AMission> it(world); it; ++it) {
        AMission* mission = *it;
        MissionInfo m;
        m.Id = mission->GetMissionId();
        m.Name = mission->GetMissionName();
        MissionDataList.Add(m);
    }
    */
}
