#include "PMDModel.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


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

    // ヘッダ情報出力
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

    file.close();

    spdlog::get("basic_logger")->info("PMD file loaded successfully.");
}