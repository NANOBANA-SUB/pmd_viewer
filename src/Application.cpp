#include "Application.h"
#include <spdlog/spdlog.h>

bool Application::isInitialize()
{
    // 初期化メソッド
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::error("Some error message with arg: {}", SDL_GetError());
        return -1;
    }
}