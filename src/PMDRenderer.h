#ifndef PMD_RENDERER_H
#define PMD_RENDERER_H

#include <vector>
#include <string>
#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>

// PMDヘッダ構造体
#pragma pack(push, 1)
struct PMDHeader
{
    float version;
    char model_name[20];
    char comment[256];
};
#pragma pack(pop)

// PMD頂点構造体
#pragma pack(push, 1)
struct PMDVertex
{
    glm::vec3 pos;          // 頂点座標
    glm::vec3 normal;       // 法線ベクトル
    glm::vec2 uv;           // uv座標
    uint16_t boneNo[2];     // ボーン番号
    uint8_t weight;         // ボーン影響度
    uint8_t edge_flag;      // 輪郭線フラグ
};
#pragma pack(pop)

// PMDマテリアル構造体
#pragma pack(1)
struct PMDMaterial
{
    glm::vec3 diffuse;      // ディフューズ
    float alpha;            // ディフューズα
    float specularity;      // スペキュラの強さ(乗算値)
    glm::vec3 specular;     // スペキュラ色
    glm::vec3 ambient;      // アンビエント色
    uint8_t toonIdx;        // トゥーン番号
    uint8_t edge_flag;      // マテリアルごとの輪郭線フラグ
    uint32_t indicesNum;    // インデックス数
    uint8_t tecFilePath[20];// テクスチャファイル名+α
};
#pragma pack()

// PMDモデルのロード
void loadPMD(const std::string& filePath, PMDHeader& header, std::vector<PMDVertex>& vertices, std::vector<uint16_t>& indices,
            std::vector<PMDMaterial>& materials);

// OpenGL用のVBOを作成
GLuint createVBO(const std::vector<PMDVertex>& vertices);

// OpenGL用のEBOを作成
GLuint createEBO(const std::vector<uint16_t>& indices);

// OpenGL用の描画処理
void render(GLuint vbo, GLuint ebo, GLuint shaderProgram, size_t vertexCount, size_t indexCount, const std::vector<PMDMaterial>& materials);

std::string loadShaderSource(const std::string& filePath);

GLuint compileShader(GLenum type, const std::string& source);

GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

#endif // PMD_RENDERER_H