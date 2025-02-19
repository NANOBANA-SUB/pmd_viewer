#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    // コピーは禁止、ムーブは許可
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void Bind() const;
    static void Unbind();
private:
    GLuint m_vao{};
};

#endif // VERTEXARRAY_H