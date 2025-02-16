#include "PMDModel.h"
#include "ShiftJis.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void PMDModel::loadPMD(std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        spdlog::get("basic_logger")->error("Failed to open file: {}", filePath);
        return;
    }

    // シグネチャの処理
    char signature[3];
    file.read(signature, sizeof(signature));
    if (std::strncmp(signature, "Pmd", 3) != 0) 
    {
        spdlog::get("basic_logger")->error("Invalid PMD file signature");
        return;
    }

    // ヘッダを読み込む
    file.read(reinterpret_cast<char*>(&m_header), sizeof(PMDHeader));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read PMD header");
        return;
    }

    // ヘッダ情報を読み込む
    spdlog::get("basic_logger")->info("Header version: {}", m_header.version);
    spdlog::get("basic_logger")->info("Model Name: {}", m_header.model_name);
    spdlog::get("basic_logger")->info("Comment: {}", m_header.comment);

    // 頂点数を読み込む
    uint32_t vertex_count;
    file.read(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read vertex count");
        return;
    }
    spdlog::get("basic_logger")->info("Vertex count: {}", vertex_count);

    // 頂点データを読み込む
    m_vertices.resize(vertex_count);
    file.read(reinterpret_cast<char*>(m_vertices.data()), vertex_count * sizeof(PMDVertex));
    for (auto& vertex : m_vertices) 
    {
        // MMDは左手系であるので，OpenGLに適用するためX軸を反転させる
        vertex.pos.x = -vertex.pos.x; // X軸を反転
    }  
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read vertex data");
        return;
    }  

    // インデックス数を読み込む
    uint32_t index_count;
    file.read(reinterpret_cast<char*>(&index_count), sizeof(uint32_t));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read index count");
        return;
    }
    spdlog::get("basic_logger")->info("Index count: {}", index_count);

    // インデックスデータを読み込む
    m_indices.resize(index_count);
    file.read(reinterpret_cast<char*>(m_indices.data()), index_count * sizeof(uint16_t));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read index data");
        return;
    }

    // マテリアル数を読み込む
    uint32_t material_count;
    file.read(reinterpret_cast<char*>(&material_count), sizeof(uint32_t));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read material count");
        return;
    }
    spdlog::get("basic_logger")->info("Material count: {}", material_count);

    // マテリアルデータを読み込む
    m_materials.resize(material_count);
    file.read(reinterpret_cast<char*>(m_materials.data()), material_count * sizeof(PMDMaterial));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read material data");
        return;
    }

    // ボーン数を読み込む
    uint16_t bone_count;
    file.read(reinterpret_cast<char*>(&bone_count), sizeof(uint16_t));
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read bone count");
        return;
    }
    spdlog::get("basic_logger")->info("Bone count: {}", bone_count);

    // ボーンデータを読み込む
    m_bones.resize(bone_count);
    file.read(reinterpret_cast<char*>(m_bones.data()), bone_count * sizeof(PMDBone));
    for (auto& bone : m_bones)
    {
        // こちらも右手系に変換
        bone.pos.x = -bone.pos.x;
    }
    if (!file) 
    {
        spdlog::get("basic_logger")->error("Failed to read bone data");
        return;
    }
    
    file.close();

    changeNodeTable();

    // 左腕のボーンノードを取得
    auto& node = m_boneNodeTable["左腕"];
    glm::vec3 pos = node.startPos;  // ボーンの開始位置

    std::cout << "Before Rotation Pos: " << glm::to_string(pos) << std::endl;

    // ローカル座標で回転を適用
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -pos);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), pos);

    // 回転行列を適用
    glm::mat4 localRotation = translateBack * rotation * translateToOrigin;

    // ボーン行列を更新 (ローカル座標のまま適用)
    recursiveMatrixMultiply(&node, localRotation);

    std::cout << "After Rotation Pos: " << glm::to_string(pos) << std::endl;

    spdlog::get("basic_logger")->info("PMD file loaded successfully.");
}

std::string PMDModel::ResolveTexPath(const std::string& relativeTexPath)
{
    // 画像データのパスを絶対パスで返す
    namespace fs = std::filesystem;
    fs::path pmdDir = fs::path(m_filePath).parent_path();
    fs::path texturePath = pmdDir / relativeTexPath;
    return texturePath.string();
}

void PMDModel::changeNodeTable()
{
    // m_bonesとm_boneNodeTableを変換する
    std::vector<std::string> boneNames(m_bones.size());

    // ボーンノードマップを作る
    for (int idx = 0; idx < m_bones.size(); idx++)
    {
        auto& bone = m_bones[idx];
        boneNames[idx] = ShiftJisToUtf8(bone.boneName);
        auto& node = m_boneNodeTable[ShiftJisToUtf8(bone.boneName)];
        node.boneIdx = idx;
        node.startPos = bone.pos;
    }

    // 親子関係を構築
    for (auto& bone : m_bones)
    {
        if (bone.parentNo >= m_bones.size()) continue;

        auto parentName = boneNames[bone.parentNo];
        m_boneNodeTable[parentName].children.emplace_back(&m_boneNodeTable[ShiftJisToUtf8(bone.boneName)]);
    }

    spdlog::get("basic_logger")->info("Registered bone names:");
    for (const auto& pair : m_boneNodeTable)
    {
        spdlog::get("basic_logger")->info(" - {}", pair.first);
    }

    m_boneMatrices.resize(m_bones.size());

    // ボーン行列の初期化
    std::fill(m_boneMatrices.begin(), m_boneMatrices.end(), glm::mat4x4(1.0));
}

void PMDModel::recursiveMatrixMultiply(BoneNode* node, glm::mat4 mat)
{
    // 回転平行移動を子ノードへ再帰的に処理する
    m_boneMatrices[node->boneIdx] *= mat;

    for (auto& cnode : node->children)
    {
        recursiveMatrixMultiply(cnode, m_boneMatrices[node->boneIdx]);
    }
}