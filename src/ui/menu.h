#pragma once

// Menu overlay, kept separate from hook.cpp on purpose:
// edit the look and the buttons here without touching hooking code.
void Menu_InitStyle();   // called once after ImGui::CreateContext()
void Menu_Render();      // called every frame from HookedPresent
void Menu_Toggle();      // flip visibility (bound to INSERT in hook.cpp)
bool Menu_IsVisible();   // true while the menu should render/receive input
