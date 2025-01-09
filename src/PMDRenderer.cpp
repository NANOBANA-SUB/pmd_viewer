#include "PMDRenderer.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

void loadPMD(const std::string& filePath, PMDHeader& header, std::vector<PMDVertex>& vertices, std::vector<uint16_t>& indices,
            std::vector<PMDMaterial>& materials)
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

    // インデックス数を読み込む
    uint32_t index_count;
    file.read(reinterpret_cast<char*>(&index_count), sizeof(uint32_t));
    if (!file) 
    {
        std::cerr << "Failed to read index count" << std::endl;
        return;
    }

    std::cout << "Index count: " << index_count << std::endl;

    // インデックスデータを読み込む
    indices.resize(index_count);
    file.read(reinterpret_cast<char*>(indices.data()), index_count * sizeof(uint16_t));
    if (!file) 
    {
        std::cerr << "Failed to read index data" << std::endl;
        return;
    }

    // マテリアル数を読み込む
    uint32_t material_count;
    file.read(reinterpret_cast<char*>(&material_count), sizeof(uint32_t));
    if (!file) 
    {
        std::cerr << "Failed to read material count" << std::endl;
        return;
    }

    std::cout << "Material count: " << material_count << std::endl;

    // マテリアルデータを読み込む
    materials.resize(material_count);
    file.read(reinterpret_cast<char*>(materials.data()), material_count * sizeof(PMDMaterial));
    if (!file) 
    {
        std::cerr << "Failed to read material data" << std::endl;
        return;
    }

    file.close();
    std::cout << "PMD file loaded successfully." << std::endl;
}

GLuint createVBO(const std::vector<PMDVertex>& vertices)
{
    GLuint vbo;

    // 頂点データをvboに送信
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PMDVertex), vertices.data(), GL_STATIC_DRAW);

    return vbo;
}

GLuint createEBO(const std::vector<uint16_t>& indices)
{
    GLuint ebo;

    // インデックスデータをeboに送信
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

    return ebo;
}

void render(GLuint vbo, GLuint ebo, GLuint shaderProgram, size_t vertexCount, size_t indexCount, const std::vector<PMDMaterial>& materials) 
{
    glUseProgram(shaderProgram);

    // シェーダーに行列を設定
    glm::mat4 rotation_y = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_x = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 model = rotation_x * rotation_y * glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    float aspectRatio = 1270.0f / 720.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.0f, 10.0f, 20.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 0.8f, 0.8f, 0.8f);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PMDVertex), (void*)offsetof(PMDVertex, uv));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // 累積オフセット
    size_t indexOffset = 0;

    for (const auto& material : materials) 
    {
        // マテリアル情報をシェーダーに送信
        glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.specularity); // スペキュラの強さ

        // 描画
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(material.indicesNum), GL_UNSIGNED_SHORT, (void*)(indexOffset * sizeof(GLushort)));

        // 次のオフセットを計算
        indexOffset += material.indicesNum;
    }

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