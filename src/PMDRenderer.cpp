#include "PMDRenderer.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void loadPMD(const std::string& filePath, PMDHeader& header, std::vector<PMDVertex>& vertices)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // ヘッダを読み込む
    file.read(reinterpret_cast<char*>(&header), sizeof(PMDHeader));

    // 頂点数を読み込む
    uint32_t vertex_count;
    file.read(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));

    // 頂点データを読み込む
    vertices.resize(vertex_count);
    file.read(reinterpret_cast<char*>(vertices.data()), vertex_count * sizeof(PMDVertex));

    file.close();
}

GLuint createVBO(const std::vector<PMDVertex>& vertices)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PMDVertex), vertices.data(), GL_STATIC_DRAW);

    return vbo;
}