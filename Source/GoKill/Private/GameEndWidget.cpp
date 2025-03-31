// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameEndWidget::QuitGame()
{
    auto pc = GetWorld()->GetFirstPlayerController();
    // 에디터일 때는 뒤에 false, 아닐 때는 true 를 넣어줘야 한다
    #if WITH_EDITOR
        UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false);
    #else
        UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, true);
    #endif
}

void UGameEndWidget::RetryGame()
{
    FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(GetWorld(), FName(*mapName));
}
