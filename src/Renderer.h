#ifndef RENDERER_H
#define RENDERER_H

#include "PMDModel.h"
#include "Shader.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Renderer
{
public:
    Renderer(const PMDModel model, const Shader shader) :m_model(model), m_shaderProgram(shader.getSheaderProgram()) 
    {
        createVBO();
        createEBO();
    }
    void render();
private:
    GLuint m_vbo, m_ebo, m_shaderProgram;
    PMDModel m_model;
    std::unordered_map<std::string, GLuint> m_textureCache;

    void createVBO();
    void createEBO();

    GLuint LoadTexture(const std::string& texturePath);
    GLuint GetOrLoadTexture(const std::string& texturePath);
};

#endif RENDERER_H