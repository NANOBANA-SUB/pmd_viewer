#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Renderer::render()
{
    glUseProgram(m_shaderProgram);

    // シェーダに行列を設定
    glm::mat4 rotation_y = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_x = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 model = m_rotation_x * m_rotation_y * glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    float aspectRatio = 1270.0f / 720.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightPos"), 1.0f, 10.0f, 20.0f);
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightColor"), 0.8f, 0.8f, 0.8f);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, uv));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // 累積オフセット
    size_t indexOffset = 0;

    for (const auto& material : m_model.get_materials()) 
    {
        // マテリアル情報をシェーダーに送信
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
        glUniform3f(glGetUniformLocation(m_shaderProgram, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
        glUniform1f(glGetUniformLocation(m_shaderProgram, "material.shininess"), material.specularity); // スペキュラの強さ

        // 描画
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(material.indicesNum), GL_UNSIGNED_SHORT, (void*)(indexOffset * sizeof(GLushort)));

        // 次のオフセットを計算
        indexOffset += material.indicesNum;
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}