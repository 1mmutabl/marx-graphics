#pragma once

#include "../vector.hpp"
#include "texture.hpp"

namespace marx::graphics
{
class sprite
{
  public:

    void load(const texture &Texture);

    void           set_size(marx::fVector2 NewSize);
    marx::fVector2 get_size() const;

    void           set_position(marx::fVector2 NewPosition);
    marx::fVector2 get_position() const;

    unsigned int get_id() const;

    sprite(const texture &Texture, marx::fVector2 Position = { 0.f, 0.f },
           marx::fVector2 Size = { 100.f, 100.f });

  private:

    unsigned int TextureID;

    marx::fVector2 Position;
    marx::fVector2 Size;
};
} // namespace marx::graphics