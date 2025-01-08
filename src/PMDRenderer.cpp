#include "PMDRenderer.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

void loadPMD(const std::string& filePath, PMDHeader& header, std::vector<PMDVertex>& vertices)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::cout << "File opened successfully: " << filePath << std::endl;

    // シグネチャを読み込む
    char signature[3];
    file.read(signature, sizeof(signature));
    if (std::strncmp(signature, "Pmd", 3) != 0) 
    {
        std::cerr << "Invalid PMD file signature" << std::endl;
        return;
    }

    // ヘッダを読み込む
    file.read(reinterpret_cast<char*>(&header), sizeof(PMDHeader));
    if (!file) 
    {
        std::cerr << "Failed to read PMD header" << std::endl;
        return;
    }

    // デバッグ出力
    std::cout << "Header version: " << header.version << std::endl;
    std::cout << "Model name: " << header.model_name << std::endl;
    std::cout << "Comment: " << header.comment << std::endl;

    // 頂点数を読み込む
    uint32_t vertex_count;
    file.read(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
    if (!file) 
    {
        std::cerr << "Failed to read vertex count" << std::endl;
        return;
    }

    std::cout << "Vertex count: " << vertex_count << std::endl;

    // 頂点データを読み込む
    vertices.resize(vertex_count);
    file.read(reinterpret_cast<char*>(vertices.data()), vertex_count * sizeof(PMDVertex));
    if (!file) 
    {
        std::cerr << "Failed to read vertex data" << std::endl;
        return;
    }

    file.close();
    std::cout << "PMD file loaded successfully." << std::endl;
}

GLuint createVBO(const std::vector<PMDVertex>& vertices)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PMDVertex), vertices.data(), GL_STATIC_DRAW);

    return vbo;
}

void render(GLuint vbo, GLuint shaderProgram, size_t vertexCount) 
{
    glUseProgram(shaderProgram);

    // シェーダーに行列を設定
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 頂点データをバインドして描画
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, uv));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_POINTS, 0, vertexCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

// シェーダファイルを読み込む
std::string loadShaderSource(const std::string& filepath) 
{
    std::ifstream file(filepath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// シェーダのコンパイル
GLuint compileShader(GLenum type, const std::string& source) 
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
        std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
    }
    return shader;
}

// シェーダープログラムの作成
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) 
{
    // シェーダーソースを読み込む
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);

    // シェーダーをコンパイル
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // シェーダープログラムをリンク
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // リンクエラーのチェック
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program Linking Failed: " << infoLog << std::endl;
    }

    // コンパイル済みシェーダーを削除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}