
#include "exports.h"
#include <windows.h>

DWORD WINAPI HelloThread(LPVOID)
{
    MessageBoxW(
        nullptr,
        L"Hello, World",
        L"GreetBox",
        MB_OK
    );

    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hInstance,
    DWORD reason,
    LPVOID reserved
)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hInstance);

        CreateThread(
            nullptr,
            0,
            HelloThread,
            nullptr,
            0,
            nullptr
        );
    }

    return TRUE;
}