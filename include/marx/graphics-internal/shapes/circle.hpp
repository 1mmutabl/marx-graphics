#pragma once

#include "../color.hpp"
#include "../vector.hpp"

namespace marx::graphics
{
class circle
{
public:

  circle(fVector2 Position = { 0, 0 }, float Radius = 50.f,
         color Color = { 255, 255, 255, 255 }, int Segments = 64);

  void     set_position(fVector2 NewPosition);
  fVector2 get_position() const;

  void  set_radius(float NewRadius);
  float get_radius() const;

  void  set_color(color NewColor);
  color get_color() const;

  void set_segments(int Segments);
  int  get_segments() const;

private:

  fVector2 Position_;
  float    Radius_;
  color    Color_;

  int Segments_;
};
} // namespace marx::graphics
