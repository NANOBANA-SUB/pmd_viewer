#include "Window.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

Window::Window()
{
    Init();
}

Window::Window(const std::string& title, int width, int height)
{
    m_data = WindowData{title, width, height};
    Init();
}

Window::~Window()
{
    Shutdown();
}

void Window::Init()
{
    // 初期化メソッド
    // ロガーの設定
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");

    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::get("basic_logger")->error("Failed to initialize SDL: {}", SDL_GetError());
        return;
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
    m_window = SDL_CreateWindow(m_data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_data.Size.Width, m_data.Size.Height, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        spdlog::get("basic_logger")->error("Failed to create window: {}", SDL_GetError());
        return;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    glewInit();

    spdlog::get("basic_logger")->info("Application is Initialized.");
}

void Window::Shutdown() const
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}