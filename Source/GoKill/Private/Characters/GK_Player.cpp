#include "Characters/GK_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

#include "shDebug.h"

AGK_Player::AGK_Player()
{
    PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true;
}

void AGK_Player::BeginPlay()
{
	Super::BeginPlay();
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

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
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
	if (!GEngine)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grab"));
}

void AGK_Player::Select(const FInputActionValue& Value)
{
	if (!GEngine)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Select"));
}

UCameraComponent* AGK_Player::GetPlayerCamera()
{
    return FollowCamera;
}

FVector AGK_Player::GetCameraLocation()
{
    return FollowCamera->GetRelativeLocation();
}

void AGK_Player::SetCameraRotation(FRotator CameraRotation_)
{
    FollowCamera->SetWorldRotation(CameraRotation_);
}
