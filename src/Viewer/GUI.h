#ifndef GUI_H
#define GUI_H

#include "FrameBuffer.h"
#include <SDL.h>
#include <imgui.h>
#include <string>
#include <IconsFontAwesome6.h>

class GUI
{
public:
    GUI();
    ~GUI();

    static void Init(SDL_Window* window, SDL_GLContext* context);
    static void LoadConfigs();
    static void Run();
    static void Render(const FrameBuffer& sceneBuffer);

    static void Print(const std::string& message);

    static void ShowMenu();
    static void ShowEntities();
    static void ShowFiles();
    static void ShowConsole();
    static void ShowScene(const FrameBuffer& sceneBuffer);
    static void ShowProperties();

    static void Shutdown();
private:
    static std::string s_log;
    static ImVec4* s_styleColors;
};
#endif // GUI_H