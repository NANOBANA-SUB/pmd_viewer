#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
private:
    GLuint m_shaderProgram;
    std::string loadShaderSource(const std::string& filePath);
    GLuint compileShader(GLenum type, const std::string& source);
    void createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
};

#endif SHADER_H