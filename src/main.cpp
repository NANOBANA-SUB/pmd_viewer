#include "PMDRenderer.h"
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("PMD Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1270, 720, SDL_WINDOW_OPENGL);
    if (!window) 
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    glewInit();

    // シェーダープログラムを作成
    GLuint shaderProgram = createShaderProgram("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");

    // PMDモデルの読み込み
    PMDHeader header;
    std::vector<PMDVertex> vertices;
    std::vector<uint16_t> indices;
    loadPMD("model/miku.pmd", header, vertices, indices);    // とりあえずハードコーディングで

    // VBO作成
    GLuint vbo = createVBO(vertices);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    bool running = true;
    SDL_Event event;
    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render(vbo, shaderProgram, vertices.size());
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

