#ifndef RENDER_H
#define RENDER_H

#include <memory>
#include <GL/glew.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "../Shader.h"
#include "../PMDModel.h"

struct RendererData
{
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ebo;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<PMDModel> m_pmdModel;
};

class Renderer
{
public:
    Renderer();

    void Render();
    const RendererData& GetData() const { return m_data; }

private:
    RendererData m_data{};

    void Init();
    void SetVariables();
    void SetupBuffers();
};
#endif // RENDER_H