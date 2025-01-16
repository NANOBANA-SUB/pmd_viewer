#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <SDL.h>
#include <GL/glew.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class Application
{
public:
    Application():m_window(nullptr), m_glContext(NULL), m_isRunning(true){}
    void run();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_isRunning;

    bool isInitialize();
};

#endif APPLICATION_H