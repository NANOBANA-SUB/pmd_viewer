#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <SDL.h>
#include <string>
#include <iostream>

struct WindowSize
{
    int Width, Height;

    WindowSize(): Width(0), Height(0){}
    WindowSize(int width, int height): Width(width), Height(height){}
};

struct WindowData
{
    std::string Title;
    WindowSize Size;

    WindowData(): Title("PMD Viewer"), Size(1280, 720){}
    WindowData(const std::string& title, int width, int height): Title(title), Size(width, height){}
};

class Window
{
public:
    Window();
    Window(const std::string& title, int width, int height);
    ~Window();

    static Window Create() { return Window{}; }
    static Window Create(const std::string& title, int width, int height) { return Window{title, width, height}; }

    void Init();

    SDL_Window* GetNativeWindow() const { return m_window; }
    const std::string& GetTitle() const { return m_data.Title; }
    WindowSize GetSize() const { return m_data.Size; }

    void Shutdown() const;
private:
    SDL_Window* m_window{};
    SDL_GLContext m_glContext{};
    WindowData m_data;
};

#endif WINDOW_H