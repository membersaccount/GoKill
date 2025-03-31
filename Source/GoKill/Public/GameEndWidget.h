// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOKILL_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 앱 종료 함수
	UFUNCTION(BlueprintCallable, Category="MenuEvent")
	void QuitGame();

    UFUNCTION(BlueprintCallable, Category="MenuEvent")
    void RetryGame();
};
