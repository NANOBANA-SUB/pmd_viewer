#ifndef RENDERER_H
#define RENDERER_H

#include "PMDModel.h"
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Renderer
{
public:
    Renderer(const PMDModel model, const Shader shader) :m_model(model), m_shaderProgram(shader.getSheaderProgram()),
    m_rotation_x(glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
    m_rotation_y(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
    m_rotation_z(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)))
    {
        createVBO();
        createEBO();
    }
    void render();
    void set_rotation_x(const float& radian) { m_rotation_x = glm::rotate(glm::mat4(1.0f), glm::radians(radian), glm::vec3(1.0f, 0.0f, 0.0f)); }
    void set_rotation_y(const float& radian) { m_rotation_y = glm::rotate(glm::mat4(1.0f), glm::radians(radian), glm::vec3(0.0f, 1.0f, 0.0f)); }
    void set_rotation_z(const float& radian) { m_rotation_z = radian; }

private:
    GLuint m_vbo, m_ebo, m_shaderProgram;
    glm::mat4 m_rotation_x, m_rotation_y, m_rotation_z;
    PMDModel m_model;

    void createVBO();
    void createEBO();
};

#endif RENDERER_H