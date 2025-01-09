#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <SDL.h>
#include <GL/glew.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class Application
{
private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_isRunning;

    bool isInitialize();
public:
    Application():m_window(nullptr), m_glContext(NULL), m_isRunning(true){}
    void run();
};

#endif APPLICATION_H