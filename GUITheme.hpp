#pragma once
#include <imgui.h>

class GuiTheme {
public:
    static void ApplySciFi() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 6.0f;
        style.FrameRounding = 4.0f;
        style.PopupRounding = 4.0f;
        style.GrabRounding = 3.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.07f, 0.12f, 0.85f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.80f, 0.80f, 0.50f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.15f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.25f, 0.40f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.12f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.20f, 0.35f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.10f, 0.30f, 0.45f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.45f, 0.65f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.55f, 0.80f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.12f, 0.25f, 0.40f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.35f, 0.55f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.45f, 0.70f, 1.00f);
    }
};