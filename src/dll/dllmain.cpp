#include <shlobj.h>
#include <string>
#include "../hook/hook.h"
#include "../memory/memory.h"
#include <windows.h>

void WriteToIni()
{
    wchar_t localAppData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) 
    {
        std::wstring iniPath = localAppData;

        iniPath += L"\\Kamiwaza\\Saved\\Config\\WindowsNoEditor\\GameUserSettings.ini";

        WritePrivateProfileStringW(
            L"/Script/Engine.GameUserSettings", 
            L"FrameRateLimit",                   
            L"60.000000",                        
            iniPath.c_str()                      
        );
    }
}

void PatchFPS()
{
    // I did not test every address to make sure everything needs to be patched,
    // but for now it will stay like this.
    WriteFloat(TARGET_FPS, 60.0f);
    WriteFloat(FPS_HALF_RATE, 30.0f);
    WriteFloat(FRAME_DELTA, 1.0f / 60.0f);
    WriteFloat(FRAME_DELTA_2, 1.0f / 60.0f);
    WriteFloat(FRAME_DELTA_3, 1.0f / 60.0f);
    WriteFloat(FRAME_TIMING_DELTA, 1.0f / 60.0f);
    WriteByte(FIXED_FRAME_BRANCH, 0xEB);
    // I probably need to look at the game using cheat engine/ghidra by myself 
    // instead of letting the AI imagine how things work (sadly I'm not that good either :( )
}


DWORD WINAPI CoolThread(LPVOID)
{
    PatchFPS();
    WriteToIni();
    PresentHook();
    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hInstance,
    DWORD reason,
    LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hInstance);

        CreateThread(
            nullptr,
            0,
            CoolThread,
            nullptr,
            0,
            nullptr);
    }
    
    if (reason == DLL_PROCESS_DETACH)
    {
        PresentUnhook();
    }
    return TRUE;
}