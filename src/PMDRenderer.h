#ifndef PMD_RENDERER_H
#define PMD_RENDERER_H

#include <vector>
#include <string>
#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>

// PMDヘッダ構造体
struct PMDHeader
{
    float version;
    char model_name[20];
    char comment[256];
};

// PMD頂点構造体
struct PMDVertex
{
    glm::vec3 pos;          // 頂点座標
    glm::vec3 normal;       // 法線ベクトル
    glm::vec3 uv;           // uv座標
    uint16_t boneNo[2];     // ボーン番号
    uint8_t weight;         // ボーン影響度
    uint8_t edge_flag;      // 輪郭線フラグ
};

// PMDモデルのロード
void loadPMD(const std::string& filePath, PMDHeader& header, std::vector<PMDVertex>& vertices);

// OpenGL用のVBOを作成
GLuint createVBO(const std::vector<PMDVertex>& vertices);

// OpenGL用の描画処理
void render(GLuint vbo, GLuint shaderProgram, size_t vertexCount);

#endif // PMD_RENDERER_H