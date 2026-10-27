#pragma once

#include <cstdint>

namespace marx::graphics
{
struct color
{
  uint8_t r = 0, g = 0, b = 0, a = 0;

  color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0)
      : r(r)
      , g(g)
      , b(b)
      , a(a)
  {
  }
};
} // namespace marx::graphics
