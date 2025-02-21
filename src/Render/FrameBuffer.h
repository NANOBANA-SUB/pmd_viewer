#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Texture.h"

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void RescaleFrameBuffer(int width, int height) const;
    void AttachTexture(int width, int height);

    void Bind() const;
    static void Unbind();

    const std::unique_ptr<Texture>& GetFrameTexture() const;
private:
    GLuint m_fbo{}, m_rbo{};
    std::unique_ptr<Texture> m_texture;
};

#endif // FRAMEBUFFER_H