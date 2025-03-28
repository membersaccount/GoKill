#include "Network/SHNetGameInstance.h"
#include <Windows.h>

USHNetGameInstance::USHNetGameInstance()
{
}

void USHNetGameInstance::Init()
{
 //   if (AllocConsole())
 //   {
 //       FILE* pCout;
 //       freopen_s(&pCout, "CONOUT$", "w", stdout);
 //       freopen_s(&pCout, "CONOUT$", "w", stderr);
 //       std::cout.clear();
 //       std::cerr.clear();
 //       std::clog.clear();
 //       std::wcout.clear();
 //       std::wcerr.clear();
 //       std::wclog.clear();

 //       std::cout << "Console Debug Start" << std::endl;
 //   }

	//console = GetConsoleWindow();

    if (network->SetGameInstanceCount != 0)
    {
		GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, TEXT("[Critical Error] GameInstance is already set"));
        return;
    }

	//network->Run();
	//network->SetGameInstance(this);
	cachedSocket = network->GetSocket();
}

void USHNetGameInstance::Shutdown()
{
    //FreeConsole();
    //SendMessage(console, WM_CLOSE, 0, 0);

	network->DestroyThread();
}
