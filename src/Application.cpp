#include "Application.h"
#include "PMDModel.h"
#include "Renderer.h"
#include "Shader.h"
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
    spdlog::get("basic_logger")->info("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
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

    SDL_Event event;

    while (m_isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                m_isRunning = false;
            if (event.type == SDL_QUIT)
                m_isRunning = false;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.render();
        SDL_GL_SwapWindow(m_window);
    }

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}