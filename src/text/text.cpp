#include "marx/graphics-internal/text/text.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "marx/graphics-internal/color.hpp"
#include "marx/graphics-internal/text/font.hpp"
#include "marx/graphics-internal/text/utf8.hpp"
#include "marx/graphics-internal/vector.hpp"

namespace marx::graphics
{
namespace
{
struct placed_glyph
{
  std::size_t  Index;
  char32_t     Codepoint;
  const glyph *Glyph;
  float        X;
  float        Advance;
  bool         Space;
};

struct row
{
  std::vector<placed_glyph> Glyphs;
};

struct caret_slot
{
  std::size_t Row;
  float       X;
};

float get_row_width(const row &Row)
{
  if (Row.Glyphs.empty())
    return 0.f;

  return Row.Glyphs.back().X + Row.Glyphs.back().Advance;
}

float get_row_offset(float BlockWidth, float RowWidth, text_alignment Alignment)
{
  switch (Alignment)
  {
    case text_alignment::Center:
      return std::max(0.f, (BlockWidth - RowWidth) / 2.f);
    case text_alignment::Right:
      return std::max(0.f, BlockWidth - RowWidth);
    default:
      return 0.f;
  }
}

void wrap_row(std::vector<row> &Rows, std::vector<caret_slot> &Slots,
              float &Pen, char32_t &Previous, float LetterSpacing)
{
  std::vector<placed_glyph> &Glyphs = Rows.back().Glyphs;

  std::size_t Break = Glyphs.size();

  for (std::size_t Position = Glyphs.size(); Position > 0; --Position)
  {
    if (Glyphs[Position - 1].Space)
    {
      Break = Position - 1;
      break;
    }
  }

  std::vector<placed_glyph> Moved;

  if (Break < Glyphs.size())
  {
    Moved.assign(Glyphs.begin() + Break + 1, Glyphs.end());
    Glyphs.erase(Glyphs.begin() + Break, Glyphs.end());
  }

  while (!Glyphs.empty() && Glyphs.back().Space)
    Glyphs.pop_back();

  Rows.emplace_back();

  Pen      = 0.f;
  Previous = 0;

  if (Moved.empty())
    return;

  const float Shift = Moved.front().X;

  for (placed_glyph &Placed : Moved)
  {
    Placed.X -= Shift;
    Slots[Placed.Index] = { Rows.size() - 1, Placed.X };
  }

  Pen      = Moved.back().X + Moved.back().Advance + LetterSpacing;
  Previous = Moved.back().Codepoint;

  Rows.back().Glyphs = std::move(Moved);
}
} // namespace

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

void text::set_max_width(float NewMaxWidth)
{
  MaxWidth_ = NewMaxWidth;
  rebuild();
}

float text::get_max_width() const
{
  return MaxWidth_;
}

void text::set_alignment(text_alignment NewAlignment)
{
  Alignment_ = NewAlignment;
  rebuild();
}

text_alignment text::get_alignment() const
{
  return Alignment_;
}

void text::set_line_spacing(float NewLineSpacing)
{
  LineSpacing_ = NewLineSpacing;
  rebuild();
}

float text::get_line_spacing() const
{
  return LineSpacing_;
}

void text::set_letter_spacing(float NewLetterSpacing)
{
  LetterSpacing_ = NewLetterSpacing;
  rebuild();
}

float text::get_letter_spacing() const
{
  return LetterSpacing_;
}

fVector2 text::get_size() const
{
  return Size_;
}

std::size_t text::get_length() const
{
  return utf8::length(String_);
}

fVector2 text::get_caret_position(std::size_t Index) const
{
  return Carets_[std::min(Index, Carets_.size() - 1)];
}

std::size_t text::get_caret_index(fVector2 Point) const
{
  float RowY = Carets_.front().y;

  for (const fVector2 &Caret : Carets_)
    if (Caret.y <= Point.y)
      RowY = Caret.y;

  std::size_t Nearest  = 0;
  float       Distance = std::numeric_limits<float>::max();

  for (std::size_t Index = 0; Index < Carets_.size(); ++Index)
  {
    if (Carets_[Index].y != RowY)
      continue;

    const float Current = std::abs(Carets_[Index].x - Point.x);

    if (Current < Distance)
    {
      Nearest  = Index;
      Distance = Current;
    }
  }

  return Nearest;
}

const std::vector<float> &text::get_vertices() const
{
  return Vertices_;
}

void text::rebuild()
{
  Vertices_.clear();
  Carets_.assign(1, Position_);
  Size_ = { 0.f, 0.f };

  if (!Font_->is_loaded() || FontSize_ <= 0.f)
    return;

  const float Scale      = FontSize_ / Font_->get_size();
  const float Ascent     = Font_->get_ascent() * Scale;
  const float LineHeight = Font_->get_line_height() * Scale;
  const float LineStep   = LineHeight * LineSpacing_;

  std::vector<char32_t> Codepoints;

  for (std::size_t Index = 0; Index < String_.size();)
    Codepoints.push_back(utf8::decode(String_, Index));

  std::vector<row>        Rows(1);
  std::vector<caret_slot> Slots(Codepoints.size() + 1);
  float                   Pen      = 0.f;
  char32_t                Previous = 0;

  for (std::size_t Index = 0; Index < Codepoints.size(); ++Index)
  {
    const char32_t Codepoint = Codepoints[Index];

    if (Codepoint == U'\n')
    {
      Slots[Index] = { Rows.size() - 1, Pen };
      Rows.emplace_back();
      Pen      = 0.f;
      Previous = 0;
      continue;
    }

    if (Codepoint < U' ')
    {
      Rows.back().Glyphs.push_back(
          { Index, Codepoint, nullptr, Pen, 0.f, false });
      Slots[Index] = { Rows.size() - 1, Pen };
      continue;
    }

    if (Previous != 0)
      Pen += Font_->get_kerning(Previous, Codepoint) * Scale;

    const glyph &Glyph   = Font_->get_glyph(Codepoint);
    const float  Advance = Glyph.Advance * Scale;

    if (MaxWidth_ > 0.f && Codepoint != U' ' && Pen > 0.f &&
        Pen + Advance > MaxWidth_)
      wrap_row(Rows, Slots, Pen, Previous, LetterSpacing_);

    Rows.back().Glyphs.push_back(
        { Index, Codepoint, &Glyph, Pen, Advance, Codepoint == U' ' });
    Slots[Index] = { Rows.size() - 1, Pen };

    Pen += Advance + LetterSpacing_;
    Previous = Codepoint;
  }

  Slots[Codepoints.size()] = { Rows.size() - 1, Pen };

  float Widest = 0.f;

  for (const row &Row : Rows)
    Widest = std::max(Widest, get_row_width(Row));

  const float BlockWidth = MaxWidth_ > 0.f ? MaxWidth_ : Widest;

  std::vector<float> Offsets(Rows.size());

  for (std::size_t Index = 0; Index < Rows.size(); ++Index)
    Offsets[Index] =
        get_row_offset(BlockWidth, get_row_width(Rows[Index]), Alignment_);

  const float r = Color_.r / 255.f;
  const float g = Color_.g / 255.f;
  const float b = Color_.b / 255.f;
  const float a = Color_.a / 255.f;

  for (std::size_t Index = 0; Index < Rows.size(); ++Index)
  {
    const float Baseline = Position_.y + Ascent + Index * LineStep;

    for (const placed_glyph &Placed : Rows[Index].Glyphs)
    {
      if (!Placed.Glyph)
        continue;

      const glyph &Glyph = *Placed.Glyph;

      if (Glyph.Size.x <= 0.f || Glyph.Size.y <= 0.f)
        continue;

      const float x0 = std::round(Position_.x + Offsets[Index] + Placed.X +
                                  Glyph.Bearing.x * Scale);
      const float y0 = std::round(Baseline - Glyph.Bearing.y * Scale);
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
  }

  Carets_.resize(Slots.size());

  for (std::size_t Index = 0; Index < Slots.size(); ++Index)
    Carets_[Index] = { Position_.x + Offsets[Slots[Index].Row] + Slots[Index].X,
                       Position_.y + Slots[Index].Row * LineStep };

  if (!Codepoints.empty())
    Size_ = { Widest, (Rows.size() - 1) * LineStep + LineHeight };
}
} // namespace marx::graphics
