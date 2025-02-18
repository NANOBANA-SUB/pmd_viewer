#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Renderer::createVAO()
{
    // vaoの作成
    glGenVertexArrays(1, &m_vao);
}

void Renderer::createVBO()
{
    // 頂点データをvboに送信
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_model.get_vertices().size() * sizeof(PMDVertex), m_model.get_vertices().data(), GL_STATIC_DRAW);
}

void Renderer::createEBO()
{
    // インデックスデータをeboに送信
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_model.get_indices().size() * sizeof(uint16_t), m_model.get_indices().data(), GL_STATIC_DRAW);
}

GLuint Renderer::LoadTexture(const std::string& texturePath)
{
    int width, height, channels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

    if (!data) return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // テクスチャデータを送信
    glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, width, height, 0,
                 channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // テクスチャパラメータ設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

GLuint Renderer::GetOrLoadTexture(const std::string& texturePath)
{
    auto it = m_textureCache.find(texturePath);
    if (it != m_textureCache.end()) 
    {
        return it->second; // キャッシュから返す
    }

    GLuint textureID = LoadTexture(texturePath);
    if (textureID != 0) 
    {
        m_textureCache[texturePath] = textureID; // キャッシュに追加
    }
    return textureID;
}

void Renderer::render()
{
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);

    // シェーダに行列を設定
    glm::mat4 rotation_y = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_x = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 model = rotation_x * rotation_y * glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    float aspectRatio = 1270.0f / 720.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightPos"), 1.0f, 10.0f, 20.0f);
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightColor"), 0.8f, 0.8f, 0.8f);

    if (!m_model.get_boneMatrices().empty()) 
    {
        GLuint boneMatrixLocation = glGetUniformLocation(m_shaderProgram, "boneMatrices");
        glUniformMatrix4fv(boneMatrixLocation, static_cast<GLsizei>(m_model.get_boneMatrices().size()), GL_FALSE, glm::value_ptr(m_model.get_boneMatrices()[0]));
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, pos));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, uv));
    glVertexAttribIPointer(3, 2, GL_UNSIGNED_SHORT, sizeof(PMDVertex), (void*)offsetof(PMDVertex, boneNo)); // boneNo[2]
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, weight)); // weight (normalized)

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // 累積オフセット
    size_t indexOffset = 0;

    for (const auto& material : m_model.get_materials()) 
    {
        // マテリアル情報をシェーダーに送信
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "material.shininess"), material.specularity); // スペキュラの強さ

        std::string texturePath = m_model.ResolveTexPath(material.getTexturePath());
        GLuint textureID = GetOrLoadTexture(texturePath);
        
        bool materialHasTexture = (textureID != 0);

        glUniform1i(glGetUniformLocation(m_shaderProgram, "useTexture"), materialHasTexture ? 1 : 0);
        
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 描画
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(material.indicesNum), GL_UNSIGNED_SHORT, (void*)(indexOffset * sizeof(GLushort)));

        // 次のオフセットを計算
        indexOffset += material.indicesNum;
    }

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}