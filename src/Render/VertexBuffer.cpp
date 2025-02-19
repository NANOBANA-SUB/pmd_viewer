#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_vbo);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_vbo);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_vbo(other.m_vbo)
{
    other.m_vbo = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
    if (this != &other) // 自己代入チェック
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = other.m_vbo;
        other.m_vbo = 0;
    }
    return *this;
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const void* data, size_t size, GLenum usage)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}