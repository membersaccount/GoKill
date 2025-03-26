#include "Network/SHNetGameInstance.h"
#include <Windows.h>

USHNetGameInstance::USHNetGameInstance()
{
}

void USHNetGameInstance::Init()
{
    HWND ConsoleWindowHandle = nullptr;

    if (AllocConsole())
    {
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        freopen_s(&pCout, "CONOUT$", "w", stderr);
        std::cout.clear();
        std::cerr.clear();
        std::clog.clear();
        std::wcout.clear();
        std::wcerr.clear();
        std::wclog.clear();

        std::cout << "Console window created!" << std::endl;

        ConsoleWindowHandle = GetConsoleWindow();
    }

    std::cout << "Hello" << std::endl;

    FreeConsole();

	network.Run();
}

void USHNetGameInstance::Shutdown()
{
	network.DestroyThread();

	//Debug::DestroyConsole(consoleHandle);
}
