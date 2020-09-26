#pragma once
#include <memory>
#include <string>
#include <glad/gl.h>

struct Texture
{
    GLuint TextureId{0};

    void LoadDefault();
    void Load(const GLenum &internalFormat, const std::string &path, const GLenum &format);

    Texture() = default;
    ~Texture();
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
};
