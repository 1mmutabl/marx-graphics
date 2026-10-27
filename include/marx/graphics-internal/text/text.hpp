#pragma once

#include <string>
#include <vector>

#include "../color.hpp"
#include "../vector.hpp"
#include "font.hpp"

namespace marx::graphics
{
class text
{
public:

  static constexpr int FloatsPerVertex = 8;

  text(font &Font, std::string String = "", fVector2 Position = { 0, 0 },
       float FontSize = 32.f, color Color = { 255, 255, 255, 255 });

  void  set_font(font &NewFont);
  font *get_font() const;

  void  set_font_size(float NewFontSize);
  float get_font_size() const;

  void               set_string(std::string NewString);
  const std::string &get_string() const;

  void     set_position(fVector2 NewPosition);
  fVector2 get_position() const;

  void  set_color(color NewColor);
  color get_color() const;

  fVector2 get_size() const;

  const std::vector<float> &get_vertices() const;

private:

  void rebuild();

  font       *Font_;
  float       FontSize_;
  std::string String_;
  fVector2    Position_;
  color       Color_;

  fVector2           Size_;
  std::vector<float> Vertices_;
};
} // namespace marx::graphics
