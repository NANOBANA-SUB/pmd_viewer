#ifndef GUI_H
#define GUI_H

#include "../Render/FrameBuffer.h"
#include <SDL.h>
#include <imgui.h>
#include <string>
#include "../Viewer/IconsFontAwesome6.h"
#include "../Render/Render.h"

class GUI
{
public:
    GUI();
    ~GUI();

    static void Init(SDL_Window* window, SDL_GLContext* context);
    static void LoadConfigs();
    static void Run();
    static void Render(const FrameBuffer& sceneBuffer, Renderer& render);

    static void Print(const std::string& message);

    static void ShowMenu(Renderer& render);
    static void ShowEntities(Renderer& render);
    static void ShowFiles();
    static void ShowConsole();
    static void ShowScene(const FrameBuffer& sceneBuffer);
    static void ShowProperties(Renderer& render);

    static std::string& GetModelPath() { return s_selectModelPath; }

    static void Shutdown();
private:
    static std::string s_log;
    static ImVec4* s_styleColors;
    static std::string s_selectModelPath;
};
#endif // GUI_H