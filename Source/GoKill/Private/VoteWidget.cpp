// Fill out your copyright notice in the Description page of Project Settings.


#include "VoteWidget.h"
#include "Components/Button.h"
#include "Network/SHNetPlayerController.h"

void UVoteWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 각 버튼 입력 이벤트에 대응하는 함수를 연결
    VoteBtn1->OnClicked.AddDynamic(this, &UVoteWidget::PressedBtn1);
    VoteBtn2->OnClicked.AddDynamic(this, &UVoteWidget::PressedBtn2);
    VoteBtn3->OnClicked.AddDynamic(this, &UVoteWidget::PressedBtn3);
    VoteBtn4->OnClicked.AddDynamic(this, &UVoteWidget::PressedBtn4);
    VoteBtn5->OnClicked.AddDynamic(this, &UVoteWidget::PressedBtn5);
}

void UVoteWidget::PressedBtn1()
{
    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(0);
    }
}

void UVoteWidget::PressedBtn2()
{
    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(1);
    }
}

void UVoteWidget::PressedBtn3()
{
    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(2);
    }
}

void UVoteWidget::PressedBtn4()
{
    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(3);
    }
}

void UVoteWidget::PressedBtn5()
{
    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(4);
    }
}
