#include <shlobj.h>
#include <string>
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


DWORD WINAPI CoolThread(LPVOID)
{
    WriteToIni();
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

    return TRUE;
}