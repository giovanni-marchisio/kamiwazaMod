#pragma once

#include <cstdint>

constexpr uintptr_t TARGET_FPS = 0x475C880;
constexpr uintptr_t FPS_HALF_RATE = 0x475C884;
constexpr uintptr_t FRAME_DELTA = 0x475C888;
constexpr uintptr_t FRAME_DELTA_2 = 0x47C1E30;
constexpr uintptr_t FRAME_DELTA_3 = 0x47C25D8;
constexpr uintptr_t FIXED_SIMULATION_DELTA = 0x047C0DD8;
constexpr uintptr_t FIXED_FRAME_BRANCH = 0xD9EEDF;
constexpr uintptr_t FRAME_TIMING_DELTA = 0x397B98C;

uintptr_t GameBase();
uintptr_t GameAddress(uintptr_t offset);


float ReadFloat(uintptr_t offset);
bool WriteFloat(uintptr_t offset, float value);
bool WriteByte(uintptr_t offset, uint8_t value);