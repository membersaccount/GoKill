#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SHNetCore.h"

#include "SHNetGameInstance.generated.h"

UCLASS()
class ACCESS_API USHNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USHNetGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

public:
	SOCKET* cachedSocket;
	int clientID;

	HWND console;

	SHNetCore* network = SHNetCore::GetInstance();
};
