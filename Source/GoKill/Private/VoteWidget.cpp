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
    SendVote(0);
}

void UVoteWidget::PressedBtn2()
{
    SendVote(1);
}

void UVoteWidget::PressedBtn3()
{
    SendVote(2);
}

void UVoteWidget::PressedBtn4()
{
    SendVote(3);
}

void UVoteWidget::PressedBtn5()
{
    SendVote(4);
}

void UVoteWidget::SendVote(int id_)
{
    if (hadVoted)
    {
        return;
    }

    ASHNetPlayerController* controller = Cast<ASHNetPlayerController>(GetWorld()->GetFirstPlayerController());
    if (controller)
    {
        controller->SendVoteData(id_);
    }

    hadVoted = true;
}
