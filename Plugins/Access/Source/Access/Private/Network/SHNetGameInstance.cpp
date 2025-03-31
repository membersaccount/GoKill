#include "Network/SHNetGameInstance.h"
#include <Windows.h>

USHNetGameInstance::USHNetGameInstance()
{
}

void USHNetGameInstance::Init()
{
    if (network->SetGameInstanceCount != 0)
    {
		GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, TEXT("[Critical Error] GameInstance is already set"));
        return;
    }

	cachedSocket = network->GetSocket();
}

void USHNetGameInstance::Shutdown()
{
	network->DestroyThread();
}
