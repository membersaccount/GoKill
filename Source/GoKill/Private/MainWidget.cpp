// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "shDebug.h"

void UMainWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMainWidget::SetMissionList(TArray<MissionInfo> MissionList)
{
    FString txt = TEXT("");

    for (auto m : MissionList) {
        txt += m.Name + TEXT(" ") + (m.Completed ? TEXT("(End)") : TEXT("(Ready)")) + TEXT("\n");
    }
    
    MissionTxt->SetText(FText::FromString(txt));
}
