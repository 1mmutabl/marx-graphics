#pragma once

#include <array>

#include "../color.hpp"
#include "../vector.hpp"

namespace marx::graphics
{
class line
{
public:

  line(std::array<fVector2, 2> Position = {
           { fVector2(0, 0), fVector2(100, 100) },
       }, float Thickness = 10.f, color Color = { 255, 255, 255, 255 });

  void set_position(std::array<fVector2, 2> NewPosition);
  void set_start_position(fVector2 NewStartPosition);
  void set_end_position(fVector2 NewEndPosition);

  std::array<fVector2, 2> get_position() const;
  fVector2                get_start_position() const;
  fVector2                get_end_position() const;

  void  set_thickness(float NewThickness);
  float get_thickness() const;

  void  set_color(color NewColor);
  color get_color() const;

private:

  std::array<fVector2, 2> Position_;
  float                   Thickness_;
  color                   Color_;
};
} // namespace marx::graphics
