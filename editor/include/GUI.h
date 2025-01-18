#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <imgui.h>
#include <string>
#include "IconsFontAwesome6.h"

class GUI
{
public:
    GUI();
    ~GUI();

    static void Init(SDL_Window* window, SDL_GLContext* context);
    static void LoadConfigs();
    static void Run();
    static void Render();
    static void Shutdown();

    static void Print(const std::string& message);

    static void ShowMenu();
    static void ShowEntities();
    static void ShowFiles();
    static void ShowConsole();
    static void ShowScene();
    static void ShowProperties();
private:
    static std::string m_log;
    static ImVec4* m_styleColors;
};

#endif GUI_H