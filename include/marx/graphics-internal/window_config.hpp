#pragma once

#include <cstdint>
#include <initializer_list>

namespace marx::graphics
{
enum class config
{
  Maximized,
  Minimized,
  Fullscreen,
  Hidden,
  Undecorated,
  Transparent,
  Floating,
  Fixed,
  Centered,
  Vsync,
  Msaa
};

class window_config
{
public:

  window_config() = default;
  window_config(std::initializer_list<config> Flags);

  window_config &add(config Flag);
  window_config &remove(config Flag);
  bool           has(config Flag) const;

private:

  std::uint32_t Flags_ = 0;
};
} // namespace marx::graphics
