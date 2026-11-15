#include "marx/graphics-internal/texture/sprite.hpp"

#include <glad/gl.h>

#include <GL/gl.h>
#include <cstdint>
#include <iostream>
#include <vector>

#include "marx/graphics-internal/texture/texture.hpp"

namespace marx::graphics
{
void sprite::load(const texture &Texture)
{
    int                  Width    = Texture.get_width();
    int                  Height   = Texture.get_height();
    int                  Channels = Texture.get_channels();
    std::vector<uint8_t> Pixels   = Texture.get_pixels();

    GLuint Channel = Channels == 3 ? GL_RGB : GL_RGBA;

    glGenTextures(1, &TextureID);

    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, Channel, Width, Height, 0, Channel,
                 GL_UNSIGNED_BYTE, Pixels.data());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void sprite::set_size(marx::fVector2 NewSize)
{
    Size = NewSize;
}
marx::fVector2 sprite::get_size() const
{
    return Size;
}

void sprite::set_position(marx::fVector2 NewPosition)
{
    Position = NewPosition;
}
marx::fVector2 sprite::get_position() const
{
    return Position;
}

unsigned int sprite::get_id() const
{
    return TextureID;
}

sprite::sprite(const texture &Texture, marx::fVector2 Position,
               marx::fVector2 Size)
    : Position(Position)
    , Size(Size)
{
    load(Texture);
}
} // namespace marx::graphics