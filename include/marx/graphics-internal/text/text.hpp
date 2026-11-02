#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../color.hpp"
#include "../vector.hpp"
#include "font.hpp"

namespace marx::graphics
{
enum class text_alignment
{
  Left,
  Center,
  Right
};

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

  void  set_max_width(float NewMaxWidth);
  float get_max_width() const;

  void           set_alignment(text_alignment NewAlignment);
  text_alignment get_alignment() const;

  void  set_line_spacing(float NewLineSpacing);
  float get_line_spacing() const;

  void  set_letter_spacing(float NewLetterSpacing);
  float get_letter_spacing() const;

  fVector2 get_size() const;

  std::size_t get_length() const;
  fVector2    get_caret_position(std::size_t Index) const;
  std::size_t get_caret_index(fVector2 Point) const;

  const std::vector<float> &get_vertices() const;

private:

  void rebuild();

  font       *Font_;
  float       FontSize_;
  std::string String_;
  fVector2    Position_;
  color       Color_;

  float          MaxWidth_      = 0.f;
  text_alignment Alignment_     = text_alignment::Left;
  float          LineSpacing_   = 1.f;
  float          LetterSpacing_ = 0.f;

  fVector2              Size_;
  std::vector<fVector2> Carets_;
  std::vector<float>    Vertices_;
};
} // namespace marx::graphics
