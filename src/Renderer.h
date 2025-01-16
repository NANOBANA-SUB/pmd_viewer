#ifndef RENDERER_H
#define RENDERER_H

#include "PMDModel.h"
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Renderer
{
public:
    Renderer(const PMDModel model, const Shader shader) :m_model(model), m_shaderProgram(shader.getSheaderProgram()) {}
    void render();
private:
    GLuint m_vbo, m_ebo, m_shaderProgram;
    PMDModel m_model;

    void createVBO();
    void createEBO();
};

#endif RENDERER_H