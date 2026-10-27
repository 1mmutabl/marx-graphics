#include "marx/graphics-internal/text/text.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "marx/graphics-internal/color.hpp"
#include "marx/graphics-internal/text/font.hpp"
#include "marx/graphics-internal/vector.hpp"

namespace
{
constexpr char32_t ReplacementCharacter = 0xFFFD;

char32_t decode_utf8(const std::string &String, std::size_t &Index)
{
  const unsigned char First = String[Index];

  std::size_t Length    = 1;
  char32_t    Codepoint = First;

  if (First >= 0xF0)
  {
    Length    = 4;
    Codepoint = First & 0x07;
  }
  else if (First >= 0xE0)
  {
    Length    = 3;
    Codepoint = First & 0x0F;
  }
  else if (First >= 0xC0)
  {
    Length    = 2;
    Codepoint = First & 0x1F;
  }
  else if (First >= 0x80)
  {
    ++Index;
    return ReplacementCharacter;
  }

  if (Index + Length > String.size())
  {
    Index = String.size();
    return ReplacementCharacter;
  }

  for (std::size_t Offset = 1; Offset < Length; ++Offset)
  {
    const unsigned char Next = String[Index + Offset];

    if ((Next & 0xC0) != 0x80)
    {
      ++Index;
      return ReplacementCharacter;
    }

    Codepoint = (Codepoint << 6) | (Next & 0x3F);
  }

  Index += Length;
  return Codepoint;
}
} // namespace

namespace marx::graphics
{
text::text(font &Font, std::string String, fVector2 Position, float FontSize,
           color Color)
    : Font_(&Font)
    , FontSize_(FontSize)
    , String_(std::move(String))
    , Position_(Position)
    , Color_(Color)
{
  rebuild();
}

void text::set_font(font &NewFont)
{
  Font_ = &NewFont;
  rebuild();
}

font *text::get_font() const
{
  return Font_;
}

void text::set_font_size(float NewFontSize)
{
  FontSize_ = NewFontSize;
  rebuild();
}

float text::get_font_size() const
{
  return FontSize_;
}

void text::set_string(std::string NewString)
{
  String_ = std::move(NewString);
  rebuild();
}

const std::string &text::get_string() const
{
  return String_;
}

void text::set_position(fVector2 NewPosition)
{
  Position_ = NewPosition;
  rebuild();
}

fVector2 text::get_position() const
{
  return Position_;
}

void text::set_color(color NewColor)
{
  Color_ = NewColor;
  rebuild();
}

color text::get_color() const
{
  return Color_;
}

fVector2 text::get_size() const
{
  return Size_;
}

const std::vector<float> &text::get_vertices() const
{
  return Vertices_;
}

void text::rebuild()
{
  Vertices_.clear();
  Size_ = { 0.f, 0.f };

  if (String_.empty() || !Font_->is_loaded() || FontSize_ <= 0.f)
    return;

  const float r = Color_.r / 255.f;
  const float g = Color_.g / 255.f;
  const float b = Color_.b / 255.f;
  const float a = Color_.a / 255.f;

  const float Scale      = FontSize_ / Font_->get_size();
  const float LineHeight = Font_->get_line_height() * Scale;

  fVector2    Pen = { Position_.x, Position_.y + Font_->get_ascent() * Scale };
  float       Width    = 0.f;
  int         Lines    = 1;
  char32_t    Previous = 0;
  std::size_t Index    = 0;

  while (Index < String_.size())
  {
    const char32_t Codepoint = decode_utf8(String_, Index);

    if (Codepoint == U'\n')
    {
      Width = std::max(Width, Pen.x - Position_.x);
      Pen   = { Position_.x, Pen.y + LineHeight };
      ++Lines;
      Previous = 0;
      continue;
    }

    if (Codepoint < U' ')
      continue;

    if (Previous != 0)
      Pen.x += Font_->get_kerning(Previous, Codepoint) * Scale;

    const glyph &Glyph = Font_->get_glyph(Codepoint);

    if (Glyph.Size.x > 0.f && Glyph.Size.y > 0.f)
    {
      const float x0 = std::round(Pen.x + Glyph.Bearing.x * Scale);
      const float y0 = std::round(Pen.y - Glyph.Bearing.y * Scale);
      const float x1 = x0 + Glyph.Size.x * Scale;
      const float y1 = y0 + Glyph.Size.y * Scale;

      const float u0 = Glyph.UvMin.x;
      const float v0 = Glyph.UvMin.y;
      const float u1 = Glyph.UvMax.x;
      const float v1 = Glyph.UvMax.y;

      std::array<float, 6 * FloatsPerVertex> Quad = {
        x0, y0, u0, v0, r, g, b, a, x1, y0, u1, v0, r, g, b, a,
        x1, y1, u1, v1, r, g, b, a, x0, y0, u0, v0, r, g, b, a,
        x1, y1, u1, v1, r, g, b, a, x0, y1, u0, v1, r, g, b, a
      };

      Vertices_.insert(Vertices_.end(), Quad.begin(), Quad.end());
    }

    Pen.x += Glyph.Advance * Scale;
    Previous = Codepoint;
  }

  Width = std::max(Width, Pen.x - Position_.x);
  Size_ = { Width, Lines * LineHeight };
}
} // namespace marx::graphics
