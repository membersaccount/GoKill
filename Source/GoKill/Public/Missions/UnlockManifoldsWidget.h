// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnlockManifoldsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOKILL_API UUnlockManifoldsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
	
public:
    TArray<int32> btnNum = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    TArray<int32> pressedNum;

    bool bSuccess = false;

    // BindWidget 배열 같은걸로 못하나..

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText1;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText2;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText3;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText4;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText5;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText6;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText7;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText8;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText9;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BtnText10;
    
    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn1;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn2;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn3;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn4;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn5;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn6;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn7;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn8;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn9;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn10;

public:
    void ResetBtn();

    void CheckBtn();

    void ShuffleTArray(TArray<int32>& Array);

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

    UFUNCTION()
    void PressedBtn6();

    UFUNCTION()
    void PressedBtn7();

    UFUNCTION()
    void PressedBtn8();

    UFUNCTION()
    void PressedBtn9();

    UFUNCTION()
    void PressedBtn10();
};
