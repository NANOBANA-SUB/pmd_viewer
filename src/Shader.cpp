#include "Shader.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <sstream>

std::string Shader::loadShaderSource(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        spdlog::get("basic_logger")->error("Failed to open file: {}", filePath);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // コンパイルエラーのチェック
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        spdlog::get("basic_logger")->error("Shader Compilation Failed: {}", infoLog);
    }
    return shader;
}

void Shader::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
    // シェーダーソースを読み込む
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);

    if (vertexSource == "" || fragmentPath == "")
        return;
        
    // シェーダーをコンパイル
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // シェーダープログラムをリンク
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    // リンクエラーのチェック
    GLint success;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        spdlog::get("basic_logger")->error("Program Linking Failed: {}", infoLog);
    }

    // コンパイル済みシェーダーを削除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}