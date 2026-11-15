#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace marx::graphics
{
class texture
{
  public:

    void load(const std::string &Path);

    int get_width() const;
    int get_height() const;
    int get_channels() const;

    std::vector<uint8_t> get_pixels() const;

    texture(std::string Path);

  private:

    int                  Width, Height, Channels;
    std::vector<uint8_t> Pixels;
};
} // namespace marx::graphics