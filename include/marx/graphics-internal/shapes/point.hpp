#pragma once

#include "../color.hpp"
#include "../vector.hpp"

namespace marx::graphics
{
struct point
{
    point(marx::fVector2 Position = { 0, 0 },
          color          Color    = { 255, 255, 255, 255 })
        : Position(Position)
        , Color(Color)
    {
    }

    marx::fVector2 Position;
    color          Color;
};
} // namespace marx::graphics