#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    // コピー禁止、ムーブは許可
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void Bind() const;
    static void Unbind();

    void SetData(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
private:
    GLuint m_vbo;
};
#endif // VERTEXBUFFER_H