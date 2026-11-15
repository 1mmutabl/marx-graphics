#include "marx/graphics-internal/texture/texture.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using string = std::string;

namespace marx::graphics
{
void texture::load(const string &Path)
{
    unsigned char *Data =
        stbi_load(Path.c_str(), &Width, &Height, &Channels, 0);

    if (!Data)
    {
        std::cerr << "Error: Failed to load image '" << Path << "\'" << '\n';
        std::exit(1);
    }

    Pixels = std::vector<uint8_t>(Data, Data + (Width * Height * Channels));

    stbi_image_free(Data);
}

int texture::get_width() const
{
    return Width;
}
int texture::get_height() const
{
    return Height;
}
int texture::get_channels() const
{
    return Channels;
}

std::vector<uint8_t> texture::get_pixels() const
{
    return Pixels;
}

texture::texture(string Path)
    : Width(0)
    , Height(0)
    , Channels(0)
{
    load(Path);
}
} // namespace marx::graphics