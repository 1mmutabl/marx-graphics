#include "marx/graphics-internal/window_config.hpp"

#include <cstdint>
#include <initializer_list>

namespace marx::graphics
{
window_config::window_config(std::initializer_list<config> Flags)
{
  for (config Flag : Flags)
    add(Flag);
}

window_config &window_config::add(config Flag)
{
  Flags_ |= std::uint32_t(1) << static_cast<unsigned int>(Flag);
  return *this;
}

window_config &window_config::remove(config Flag)
{
  Flags_ &= ~(std::uint32_t(1) << static_cast<unsigned int>(Flag));
  return *this;
}

bool window_config::has(config Flag) const
{
  return (Flags_ & (std::uint32_t(1) << static_cast<unsigned int>(Flag))) != 0;
}
} // namespace marx::graphics
