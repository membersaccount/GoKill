// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MissionData.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOKILL_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    void SetMissionList(TArray<MissionInfo> MissionList);

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UTextBlock* MissionTxt;
};
