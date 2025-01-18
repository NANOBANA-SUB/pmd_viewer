#include "Application.h"
#include "PMDModel.h"
#include "Renderer.h"
#include "Shader.h"
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>

bool Application::isInitialize()
{
    // 初期化メソッド
    // ロガーの設定
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");

    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::get("basic_logger")->error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    // OpenGL設定
    // コアOpenGLプロファイル
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // RGBA各チャネル8ビットのカラーバッファ
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // Zバッファのビット数
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // ダブルバッファ
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // ハードウェアアクセラレーション
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // windowを作成
    m_window = SDL_CreateWindow("PMD Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        spdlog::get("basic_logger")->error("Failed to create window: {}", SDL_GetError());
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    glewInit();

    spdlog::get("basic_logger")->info("Application is Initialized.");
    return true;
}

void Application::run()
{
    if (!isInitialize()) return;

    std::string filePath = "model/miku.pmd";
    PMDModel miku = PMDModel(filePath);
    std::string vertexPath = "shader/vertex_shader.glsl";
    std::string fragmentPath = "shader/fragment_shader.glsl";
    Shader shader = Shader(vertexPath, fragmentPath);
    Renderer renderer = Renderer(miku, shader);

    // 背景を白に
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Zバッファ有効
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // ImGui init
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init();

    SDL_Event event;

    while (m_isRunning)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                m_isRunning = false;
            if (event.type == SDL_QUIT)
                m_isRunning = false;
        }

        // Gui
        {
            ImGui::Begin("Rotation Settings");

            static float x = 20.0f;
            if (ImGui::SliderFloat("rotate X", &x, 0.0f, 360.0f)) renderer.set_rotation_x(x);

            static float y = 180.0f;
            if (ImGui::SliderFloat("rotate Y", &y, 0.0f, 360.0f)) renderer.set_rotation_y(y);

            ImGui::End();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(m_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}