#include "marx/graphics-internal/shapes/line.hpp"

namespace marx::graphics
{
line::line(std::array<fVector2, 2> Position, float Thickness, color Color)
{
  set_position(Position);
  set_thickness(Thickness);
  set_color(Color);
}

void line::set_position(std::array<fVector2, 2> NewPosition)
{
  set_start_position(NewPosition.at(0));
  set_end_position(NewPosition.at(1));
}
void line::set_start_position(fVector2 NewStartPosition)
{
  fVector2 &x = Position_.at(0);
  x           = NewStartPosition;
}
void line::set_end_position(fVector2 NewEndPosition)
{
  fVector2 &x = Position_.at(1);
  x           = NewEndPosition;
}

std::array<fVector2, 2> line::get_position() const
{
  return Position_;
}

fVector2 line::get_start_position() const
{
  fVector2 x = Position_.at(0);
  return x;
}

fVector2 line::get_end_position() const
{
  fVector2 x = Position_.at(1);
  return x;
}

void line::set_thickness(float NewThickness)
{
  Thickness_ = NewThickness;
}

float line::get_thickness() const
{
  return Thickness_;
}

void line::set_color(color NewColor)
{
  Color_ = NewColor;
}

color line::get_color() const
{
  return Color_;
}

} // namespace marx::graphics
