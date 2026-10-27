#include "marx/graphics-internal/shapes/circle.hpp"

namespace marx::graphics
{
circle::circle(fVector2 Position, float Radius, color Color, int Segments)
{
  set_position(Position);
  set_radius(Radius);
  set_color(Color);
  set_segments(Segments);
}

void circle::set_position(fVector2 NewPosition)
{
  Position_ = NewPosition;
}

fVector2 circle::get_position() const
{
  return Position_;
}

void circle::set_radius(float NewRadius)
{
  Radius_ = NewRadius;
}

float circle::get_radius() const
{
  return Radius_;
}

void circle::set_color(color NewColor)
{
  Color_ = NewColor;
}

color circle::get_color() const
{
  return Color_;
}

void circle::set_segments(int Segments)
{
  Segments_ = Segments;
}
int circle::get_segments() const
{
  return Segments_;
}
} // namespace marx::graphics
