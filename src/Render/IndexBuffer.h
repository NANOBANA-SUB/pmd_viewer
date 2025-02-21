#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <GL/glew.h>

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    // コピー禁止、ムーブは許可
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void Bind() const;
    static void Unbind();

    void SetData(const void* indices_data, size_t indices_size, GLenum usage = GL_STATIC_DRAW);
private:
    GLuint m_ebo{};
};

#endif // INDEXBUFFER_H