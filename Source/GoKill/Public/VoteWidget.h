// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOKILL_API UVoteWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VoteBtn1;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VoteBtn2;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VoteBtn3;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VoteBtn4;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VoteBtn5;

public:
    UFUNCTION()
    void PressedBtn1();

    UFUNCTION()
    void PressedBtn2();

    UFUNCTION()
    void PressedBtn3();

    UFUNCTION()
    void PressedBtn4();

    UFUNCTION()
    void PressedBtn5();
};
