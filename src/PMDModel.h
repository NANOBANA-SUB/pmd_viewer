#ifndef PMD_Model_H
#define PMD_Model_H

#include <vector>
#include <string>
#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>

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
    uint8_t texFilePath[20];// テクスチャファイル名+α

    // テクスチャの相対パスを取得
    std::string getTexturePath() const 
    {
        return std::string(reinterpret_cast<const char*>(texFilePath), 
                           strnlen(reinterpret_cast<const char*>(texFilePath), 20));
    }
};
#pragma pack()

#pragma pack(1)
// PMDのボーン構造体
struct PMDBone
{
    char boneName[20];      // ボーン名
    uint16_t parentNo;      // 親ボーン番号
    uint16_t nextNo;        // 先端のボーン番号
    uint8_t type;           // ボーン種別
    uint16_t ikBoneNo;      // IKボーン番号
    glm::vec3 pos;          // ボーンの基準点座標
};
#pragma pack()

struct BoneNode
{
    int boneIdx;                        // ボーンインデックス
    glm::vec3 startPos;                 // ボーン基準点(回転の軸)
    glm::vec3 endPos;                   // ボーン先端点
    std::vector<BoneNode*> children;    // 子ノード
};

class PMDModel
{
public:
    PMDModel(std::string& filePath) { loadPMD(filePath); m_filePath = filePath; }
    
    std::string ResolveTexPath(const std::string& relativeTexPath);

    const std::vector<PMDVertex>& get_vertices() const { return m_vertices; }
    const std::vector<uint16_t>& get_indices() const { return m_indices; }
    const std::vector<PMDMaterial>& get_materials() const { return m_materials; }
    const std::string& get_filePath() const { return m_filePath; }
    const std::vector<glm::mat4x4> get_boneMatrices() const { return m_boneMatrices; }
private:
    PMDHeader m_header;                     // ヘッダ
    std::vector<PMDVertex> m_vertices;      // 頂点データのリスト
    std::vector<uint16_t> m_indices;        // インデックスデータのリスト
    std::vector<PMDMaterial> m_materials;   // マテリアルデータのリスト
    std::vector<PMDBone> m_bones;           // ボーンのリスト
    std::string m_filePath;
    std::map<std::string, BoneNode> m_boneNodeTable;
    std::vector<glm::mat4x4> m_boneMatrices;
    
    void loadPMD(std::string& filePath);
    void changeNodeTable();
    void recursiveMatrixMultiply(BoneNode* node, glm::mat4 mat);
};

#endif // PMD_Model_H