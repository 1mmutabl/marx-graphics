#include "marx/graphics-internal/shapes/rectangle.hpp"

namespace marx::graphics
{
rectangle::rectangle(fVector2 Position, fVector2 Size, color Color,
                     float Rotation)
{
  set_position(Position);
  set_size(Size);
  set_color(Color);
  set_rotation(Rotation);
}

void rectangle::set_position(fVector2 NewPosition)
{
  Position_ = NewPosition;
}
fVector2 rectangle::get_position() const
{
  return Position_;
}

void rectangle::set_size(fVector2 NewSize)
{
  Size_ = NewSize;
}
fVector2 rectangle::get_size() const
{
  return Size_;
}

void rectangle::set_color(color NewColor)
{
  Color_ = NewColor;
}
color rectangle::get_color() const
{
  return Color_;
}

void rectangle::set_rotation(float NewRotation)
{
  Rotation_ = NewRotation;
}
float rectangle::get_rotation() const
{
  return Rotation_;
}
} // namespace marx::graphics
