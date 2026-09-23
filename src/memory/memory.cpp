#include "memory.h"

#include <windows.h>

uintptr_t GameBase()
{
    return reinterpret_cast<uintptr_t>(
        GetModuleHandleW(nullptr)
    );
}

uintptr_t GameAddress(uintptr_t offset)
{
    return GameBase() + offset;
}

bool WriteFloat(uintptr_t offset, float value)
{
    uintptr_t address = GameAddress(offset);

    DWORD oldProtect;

    if (!VirtualProtect(
        reinterpret_cast<void*>(address),
        sizeof(float),
        PAGE_READWRITE,
        &oldProtect
    ))
    {
        return false;
    }

    *reinterpret_cast<float*>(address) = value;

    VirtualProtect(
        reinterpret_cast<void*>(address),
        sizeof(float),
        oldProtect,
        &oldProtect
    );

    return true;
}

bool WriteByte(uintptr_t offset, uint8_t value)
{
    uintptr_t address = GameAddress(offset);

    DWORD oldProtect;

    if (!VirtualProtect(
        reinterpret_cast<void*>(address),
        sizeof(uint8_t),
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    ))
    {
        return false;
    }

    *reinterpret_cast<uint8_t*>(address) = value;

    FlushInstructionCache(
        GetCurrentProcess(),
        reinterpret_cast<void*>(address),
        sizeof(uint8_t)
    );

    DWORD dummy;

    if (!VirtualProtect(
        reinterpret_cast<void*>(address),
        sizeof(uint8_t),
        oldProtect,
        &dummy
    ))
    {
        return false;
    }

    return true;
}