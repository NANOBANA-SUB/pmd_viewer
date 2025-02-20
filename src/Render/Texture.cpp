#include "Texture.h"

Texture::Texture()
{
    glGenTextures(1, &m_Texture);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Texture);
}

Texture::Texture(Texture&& other) noexcept
    :m_Texture(other.m_Texture)
{
    other.m_Texture = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        glDeleteTextures(1, &m_Texture);
        m_Texture = other.m_Texture;
        other.m_Texture = 0;
    }
    return *this;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateFromImage(const std::string imagePath)
{
    unsigned char* data = stbi_load(imagePath.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

    if (!data) return;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}