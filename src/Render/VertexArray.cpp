#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vao);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_vao(other.m_vao)
{
    other.m_vao = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = other.m_vao;
        other.m_vao = 0;
    }
    return *this;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vao);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}