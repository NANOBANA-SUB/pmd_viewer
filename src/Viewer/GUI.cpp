#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <nfd.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "GUI.h"
#include "../ShiftJis.h"

std::string GUI::s_log;
std::string GUI::s_selectModelPath;
ImVec4* GUI::s_styleColors;

int   GUI::s_gizmoOperation = ImGuizmo::TRANSLATE;
int   GUI::s_gizmoMode      = ImGuizmo::LOCAL;
bool  GUI::s_useSnap        = false;
float GUI::s_snapTranslate[3] = {0.5f, 0.5f, 0.5f};
float GUI::s_snapRotate       = 5.0f;   // 5°
float GUI::s_snapScale[3]     = {0.1f, 0.1f, 0.1f};

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

    float baseFontSize = 18.0f;
    float iconFontSize = baseFontSize * 2.0f / 2.4f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    io.Fonts->AddFontFromFileTTF("fonts/NotoSansJP-Regular.ttf", baseFontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());

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

    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void GUI::Render(const FrameBuffer& sceneBuffer, Renderer& render)
{
    ShowConsole();
    ShowEntities(render);
    ShowProperties(render);
    ShowMenu(render);
    ShowFiles();
    ShowScene(render);

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

    if (ImGui::Button("Clear"))
    {
        s_log.clear();
    }

    ImGui::SameLine();

    if (ImGui::Button("Debug"))
    {
        Print("Debug message");
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Text("%s", s_log.c_str());

    ImGui::End();
}

void GUI::ShowEntities(Renderer& render)
{
    ImGui::Begin(ICON_FA_CUBE" Entities");

    std::string modelName = ShiftJisToUtf8(render.GetData().m_pmdModel->get_pmdHeader().model_name);
    if (!modelName.empty())
    {
        ImGui::CollapsingHeader(modelName.c_str());
    }
    ImGui::End();
}

void GUI::ShowProperties(Renderer& render)
{
    ImGui::Begin(ICON_FA_BARS_STAGGERED" Properties");

    if (ImGui::CollapsingHeader("Transform"))
    {
        ImGui::BeginGroup();
        glm::vec3 position = render.GetData().m_pmdModel->get_position();
        glm::vec3 rotation = render.GetData().m_pmdModel->get_rotation();

        // 変更を受け取るためのfloat配列
        float pos[3] = { position.x, position.y, position.z };
        float rot[3] = { rotation.x, rotation.y, rotation.z };
        
        // ImGui の DragFloat3 に渡す
        if (ImGui::DragFloat3("Position", pos, 0.2f))
            render.GetData().m_pmdModel->set_position(glm::vec3(pos[0], pos[1], pos[2]));
        if (ImGui::DragFloat3("Rotation", rot, 0.2f)) 
            render.GetData().m_pmdModel->set_rotation(glm::vec3(rot[0], rot[1], rot[2]));
        
            // ---- ギズモ設定 ----
        ImGui::SeparatorText("Gizmo");
        if (ImGui::RadioButton("Translate (W)", GUI::s_gizmoOperation == ImGuizmo::TRANSLATE)) GUI::s_gizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate (E)",    GUI::s_gizmoOperation == ImGuizmo::ROTATE))    GUI::s_gizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale (R)",     GUI::s_gizmoOperation == ImGuizmo::SCALE))     GUI::s_gizmoOperation = ImGuizmo::SCALE;

        if (ImGui::RadioButton("Local",  GUI::s_gizmoMode == ImGuizmo::LOCAL))  GUI::s_gizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World",  GUI::s_gizmoMode == ImGuizmo::WORLD))  GUI::s_gizmoMode = ImGuizmo::WORLD;

        ImGui::Checkbox("Snap", &GUI::s_useSnap);
        if (GUI::s_useSnap) {
            if (GUI::s_gizmoOperation == ImGuizmo::TRANSLATE)
                ImGui::DragFloat3("Snap (T)", GUI::s_snapTranslate, 0.1f);
            else if (GUI::s_gizmoOperation == ImGuizmo::ROTATE)
                ImGui::DragFloat("Snap (deg)", &GUI::s_snapRotate, 1.0f);
            else if (GUI::s_gizmoOperation == ImGuizmo::SCALE)
                ImGui::DragFloat3("Snap (S)", GUI::s_snapScale, 0.01f);
        }

        // キーバインド（フォーカス時のみ）
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_W)) GUI::s_gizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_E)) GUI::s_gizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R)) GUI::s_gizmoOperation = ImGuizmo::SCALE;
        }

        ImGui::EndGroup();
    }
    ImGui::End();
}

void GUI::ShowMenu(Renderer& render)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) 
            {
                nfdchar_t *outPath = NULL;
                const nfdchar_t *filterList = "pmd";
                if (NFD_OpenDialog(filterList, NULL, &outPath))
                {
                    GUI::s_selectModelPath = std::string(outPath);
                    Print("Selected file: " + std::string(outPath));
                    render.Init(outPath);
                    free(outPath);
                }
            }
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

static void GlmToFloat16(const glm::mat4& m, float out[16]) { memcpy(out, &m[0][0], sizeof(float)*16); }
static glm::mat4 Float16ToGlm(const float in[16]) { glm::mat4 m; memcpy(&m[0][0], in, sizeof(float)*16); return m; }

void GUI::ShowScene(Renderer& render)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin(ICON_FA_CLAPPERBOARD" Scene");
    {
        ImGui::Image(
            (ImTextureID)(render.GetData().m_fbo->GetFrameTexture()->GetTextureID()),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        const ImVec2 winPos  = ImGui::GetWindowPos();
        const ImVec2 cursor  = ImGui::GetCursorPos(); // 画像の後なので右下端
        const ImVec2 winSize = ImGui::GetWindowSize();
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        ImGuizmo::SetOrthographic(false); // 透視投影の場合
        ImGuizmo::SetRect(winPos.x, winPos.y, winSize.x, winSize.y);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        float aspectRatio = 1270.0f / 720.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

        float viewM[16], projM[16];
        GlmToFloat16(view, viewM);
        GlmToFloat16(projection, projM);

        RendererData& data = render.GetData();

        glm::vec3 modelPos = data.m_pmdModel->get_position();
        glm::vec3 modelRot = data.m_pmdModel->get_rotation();
        
        // 回転が度の場合はラジアンに直す
        const bool rotationIsDegree = true; // 必要に応じて変更
        glm::vec3 rotRad = rotationIsDegree ? glm::radians(modelRot) : modelRot;

        glm::mat4 model =
            glm::translate(glm::mat4(1.0f), modelPos) *
            glm::yawPitchRoll(rotRad.y, rotRad.x, rotRad.z);

        float modelM[16];
        GlmToFloat16(model, modelM);

        const float* snap =
            (s_useSnap
                ? (s_gizmoOperation == ImGuizmo::TRANSLATE ? s_snapTranslate
                   : s_gizmoOperation == ImGuizmo::ROTATE   ? &s_snapRotate
                   : s_snapScale)
                : nullptr);

        if (ImGui::IsWindowHovered()) {
            if (ImGuizmo::Manipulate(viewM, projM,
                                     (ImGuizmo::OPERATION)s_gizmoOperation,
                                     (ImGuizmo::MODE)s_gizmoMode,
                                     modelM, nullptr, snap))
            {
                // 変更を分解して自前のトランスフォームに反映
                float t[3], r[3], sc[3];
                ImGuizmo::DecomposeMatrixToComponents(modelM, t, r, sc);

                // r は度単位で返るため、プロジェクトの単位に合わせる
                glm::vec3 newPos(t[0], t[1], t[2]);
                glm::vec3 newRotDeg(r[0], r[1], r[2]);
                glm::vec3 newRot = rotationIsDegree ? newRotDeg : glm::radians(newRotDeg);

                data.m_pmdModel->set_position(newPos);
                data.m_pmdModel->set_rotation(newRot);
                // scale を持たせたいなら set_scale(newScale) も用意して反映
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}