#include "menu.h"
#include "imgui.h"

static bool g_ShowMenu = true;
static int g_Count = 0;

void Menu_InitStyle()
{
    ImGui::StyleColorsDark();

    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8.0f;
    s.FrameRounding = 4.0f;
    s.ScrollbarRounding = 4.0f;
    s.GrabRounding = 4.0f;
    s.WindowBorderSize = 1.0f;
    s.FrameBorderSize = 0.0f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.14f, 1.00f);
    c[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.22f, 0.32f, 1.00f);
}

void Menu_Toggle()
{
    g_ShowMenu = !g_ShowMenu;
}

bool Menu_IsVisible()
{
    return g_ShowMenu;
}

void Menu_Render()
{
    if (!g_ShowMenu)
        return;

    ImGui::SetNextWindowSize(ImVec2(500.0f, 400.0f));
    ImGui::Begin("wip wip wip wip");

    if (ImGui::Button("click"))
    {
        g_Count++;
    }
    ImGui::Text("%d", g_Count);

    // I'm still trying to learn about all this imgui stuff and hook and dummy etc etc etc
    // sorry, this is a frankestein of a code.

    static float f = 0.5f;
    ImGui::SliderFloat("example", &f, 0.0f, 1.0f);

    if (ImGui::BeginTabBar("tabs"))
    {
        if (ImGui::BeginTabItem("one"))
        {
            ImGui::Text("yo");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("two"))
        {
            ImGui::Text("yoyo");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
