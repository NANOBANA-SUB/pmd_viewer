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
        createVAO();
        createVBO();
        createEBO();

        // 頂点属性の設定
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, uv));

        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 2, GL_UNSIGNED_SHORT, sizeof(PMDVertex), (void*)offsetof(PMDVertex, boneNo));

        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, weight));

        glBindVertexArray(0); // VAO のバインド解除
    }
    void render();
private:
    GLuint m_vao, m_vbo, m_ebo, m_shaderProgram;
    PMDModel m_model;
    std::unordered_map<std::string, GLuint> m_textureCache;

    void createVAO();
    void createVBO();
    void createEBO();

    GLuint LoadTexture(const std::string& texturePath);
    GLuint GetOrLoadTexture(const std::string& texturePath);
};

#endif RENDERER_H