#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    // コピーは禁止、ムーブは許可
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind() const;
    static void Unbind();
    void GenerateFromImage(const std::string imagePath);

    GLuint GetTextureID() const { return m_Texture; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetNrChannels() const { return m_NrChannels; }

private:
    GLuint m_Texture{};
    int m_Width{}, m_Height{}, m_NrChannels{};
};

#endif // TEXTURE_H