#pragma once

#include "../color.hpp"
#include "../vector.hpp"

namespace marx::graphics
{
class rectangle
{
public:

  rectangle(fVector2 Position = { 0, 0 }, fVector2 Size = { 100, 100 },
            color Color = { 255, 255, 255, 255 }, float Rotation = 0.f);

  void     set_position(fVector2 NewPosition);
  fVector2 get_position() const;

  void     set_size(fVector2 NewSize);
  fVector2 get_size() const;

  void  set_color(color NewColor);
  color get_color() const;

  void  set_rotation(float NewRotation);
  float get_rotation() const;

private:

  fVector2 Position_;
  fVector2 Size_;
  color    Color_;
  float    Rotation_;
};
} // namespace marx::graphics
