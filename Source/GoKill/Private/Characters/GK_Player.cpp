#include "Characters/GK_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

#include "shDebug.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "../../../../Plugins/Runtime/XRBase/Source/XRBase/Public/HeadMountedDisplayFunctionLibrary.h"

AGK_Player::AGK_Player()
{
    PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true;


    // 모션 컨트롤러 컴포넌트 추가
    LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
    LeftHand->SetupAttachment(RootComponent);
    LeftHand->SetTrackingMotionSource(TEXT("Left"));

    LeftHandSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandSK"));
    LeftHandSK->SetupAttachment(LeftHand);

    ConstructorHelpers::FObjectFinder<USkeletalMesh>TempLeftHand(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
    if (TempLeftHand.Succeeded()) {
        LeftHandSK->SetSkeletalMesh(TempLeftHand.Object);
    }

    RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
    RightHand->SetupAttachment(RootComponent);
    RightHand->SetTrackingMotionSource(TEXT("Right"));

    RightHandSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandSK"));
    RightHandSK->SetupAttachment(RightHand);

    ConstructorHelpers::FObjectFinder<USkeletalMesh>TempRightHand(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
    if (TempRightHand.Succeeded()) {
        RightHandSK->SetSkeletalMesh(TempRightHand.Object);
    }
}

void AGK_Player::BeginPlay()
{
	Super::BeginPlay();

    // HMD 가 연결되어 있으면, HMD 의 Tracking 위치를 조절
    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
        // EHMDTrackingOrigin::View 로 하면 원래 카메라가 설치되어 있는 높이
        UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
        // 초기 설정 값을 Yaw 값 외의 값을 내부적으로 조정해놔준다. 인자값에 숫자를 넣으면 해당 Yaw 값 (디폴트 : 0) 적용된다.
        // 뷰 위치 (재)조정 느낌
        UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
    }
}

void AGK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (PlayerController == nullptr)
        return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (Subsystem == nullptr)
    {
        return;
    }

    Subsystem->AddMappingContext(DefaultMappingContext, 1);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGK_Player::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGK_Player::Look);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AGK_Player::Grab);
	EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &AGK_Player::Select);
}

void AGK_Player::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

    AddMovementInput(FollowCamera->GetForwardVector() * MovementVector.X + FollowCamera->GetRightVector() * MovementVector.Y);
}

void AGK_Player::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AGK_Player::Grab(const FInputActionValue& Value)
{
    Called();
}

void AGK_Player::Select(const FInputActionValue& Value)
{
    Called();
}

UCameraComponent* AGK_Player::GetPlayerCamera()
{
    return FollowCamera;
}

FVector AGK_Player::GetCameraLocation()
{
    return FollowCamera->GetComponentLocation();
}

void AGK_Player::SetCameraRotation(FRotator CameraRotation_)
{
    FollowCamera->SetWorldRotation(CameraRotation_);
}
