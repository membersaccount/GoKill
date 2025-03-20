// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayer.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	// ī�޶� ��ġ�� ���߿� �����Ұű⿡ �߿����� �ʴ�

	// ��Ʈ�ѷ��� ȸ���� ī�޶� ��� ȸ���ϰڴ�
	VRCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pc = GetWorld()->GetFirstPlayerController();

	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(IMC_VR, 1);
		}
	}

	auto InputSystem = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (InputSystem) {
		InputSystem->BindAction(IA_VRMove, ETriggerEvent::Triggered, this, &ATestPlayer::Move);
	}
}

void ATestPlayer::Move(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();

	// ���� ���Ⱚ Direction �� ī�޶� ���� ���Ⱚ���� ��ȯ
	//FVector Direction = FVector(dir.X, dir.Y, 0);
	//VRCamera->GetComponentTransform().TransformVector(Direction);

	// ī�޶� ���� �������� �̵�
	AddMovementInput(VRCamera->GetForwardVector() * dir.X + VRCamera->GetRightVector() * dir.Y);
}

