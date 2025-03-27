#pragma once

#include "CoreMinimal.h"
#include "Characters/GK_CharacterBase.h"
#include <Windows.h>
#include "GK_Player.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraComponent;

UCLASS()
class GOKILL_API AGK_Player : public AGK_CharacterBase
{
	GENERATED_BODY()
	
public:
	AGK_Player();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Grab(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);

public: // Camera Settings
    UCameraComponent* GetPlayerCamera();
    FVector GetCameraLocation();
    void SetCameraRotation(FRotator CameraRotation_);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // 모션 컨트롤러 등록
	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* LeftHand;

	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* RightHand;

	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* LeftHandSK;

	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* RightHandSK;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectAction;

};
