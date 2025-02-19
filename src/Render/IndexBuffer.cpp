#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_ebo);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ebo);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_ebo(other.m_ebo)
{
    other.m_ebo = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = other.m_ebo;
        other.m_ebo = 0;
    }
    return *this;
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(const void* data, size_t size, GLenum usage)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}