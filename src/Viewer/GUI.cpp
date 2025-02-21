#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "GUI.h"

std::string GUI::s_log;
ImVec4* GUI::s_styleColors;

GUI::GUI(){}

void GUI::Init(SDL_Window* window, SDL_GLContext* context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    LoadConfigs();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();
}

void GUI::LoadConfigs()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.IniFilename = "imgui.ini";

    float baseFontSize = 14.0f;
    float iconFontSize = baseFontSize * 2.0f / 2.4f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    io.Fonts->AddFontFromFileTTF("fonts/Ruda-Bold.ttf", baseFontSize);

    static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF("fonts/" FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges );

    ImGui::StyleColorsDark();

    ImGuiStyle* style = &ImGui::GetStyle();
    s_styleColors = style->Colors;

    style->WindowMenuButtonPosition = ImGuiDir_None;
    style->WindowBorderSize = 0.0f;
    style->WindowMenuButtonPosition = ImGuiDir_Right;
    style->GrabRounding = 4.0f;
    style->WindowRounding = 6.0f;
    style->FrameRounding = 4.0f;
    style->FramePadding = ImVec2(5.0f, 5.0f);
    style->PopupBorderSize = 0.0f;
    style->PopupRounding = 4.0f;
    style->SeparatorTextPadding = ImVec2(5.0f, 5.0f);
    style->TabBarBorderSize = 2.0f;

    s_styleColors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    s_styleColors[ImGuiCol_DockingEmptyBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_Border] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
    s_styleColors[ImGuiCol_FrameBgHovered] = ImVec4(0.03f, 0.03f, 0.03f, 0.8f);
    s_styleColors[ImGuiCol_FrameBgActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.0f);
    s_styleColors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);
    s_styleColors[ImGuiCol_Header] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
    s_styleColors[ImGuiCol_HeaderHovered] = ImVec4(0.08f, 0.42f, 0.14f, 0.8f);
    s_styleColors[ImGuiCol_HeaderActive] = ImVec4(0.08f, 0.42f, 0.14f, 1.0f);
    s_styleColors[ImGuiCol_Tab] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_TabHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
    s_styleColors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    s_styleColors[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    s_styleColors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    s_styleColors[ImGuiCol_CheckMark] = ImVec4(0.08f, 0.42f, 0.14f, 1.0f);
    s_styleColors[ImGuiCol_SliderGrab] = ImVec4(0.08f, 0.42f, 0.14f, 0.8f);
    s_styleColors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.42f, 0.14f, 1.0f);
    s_styleColors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    s_styleColors[ImGuiCol_ButtonHovered] = ImVec4(0.08f, 0.42f, 0.14f, 0.5f);
    s_styleColors[ImGuiCol_ButtonActive] = ImVec4(0.08f, 0.42f, 0.14f, 1.0f);
    s_styleColors[ImGuiCol_TextSelectedBg] = ImVec4(0.08f, 0.42f, 0.14f, 0.35f);
}

void GUI::Run()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void GUI::Render(const FrameBuffer& sceneBuffer)
{
    ShowConsole();
    ShowEntities();
    ShowProperties();
    ShowMenu();
    ShowFiles();
    ShowScene(sceneBuffer);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Print(const std::string& message)
{
    s_log += message + "\n";
}

void GUI::ShowConsole()
{
    ImGui::Begin(ICON_FA_TERMINAL" Console");

    if(ImGui::Button("Clear"))
    {
        s_log.clear();
    }

    ImGui::SameLine();

    if(ImGui::Button("Debug"))
    {
        Print("Debug message");
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Text("%s", s_log.c_str());

    ImGui::End();
}

void GUI::ShowEntities()
{
    ImGui::Begin(ICON_FA_CUBE" Entities");

    ImGui::End();
}

void GUI::ShowProperties()
{
    ImGui::Begin(ICON_FA_BARS_STAGGERED" Properties");

    ImGui::End();
}

void GUI::ShowMenu()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  { }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void GUI::ShowFiles()
{
    ImGui::Begin(ICON_FA_FOLDER" Files");

    ImGui::End();
}

void GUI::ShowScene(const FrameBuffer& sceneBuffer)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin(ICON_FA_CLAPPERBOARD" Scene");
    {
        ImGui::Image(
            (ImTextureID)sceneBuffer.GetFrameTexture()->GetTextureID(),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::End();
    ImGui::PopStyleVar();
}