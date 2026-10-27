#pragma once

#include <string>
#include <sys/types.h>
#include <unordered_map>

#include "../vector.hpp"

struct FT_FaceRec_;

namespace marx::graphics
{
struct glyph
{
  fVector2 Size;
  fVector2 Bearing;
  float    Advance = 0.f;
  fVector2 UvMin;
  fVector2 UvMax;
};

class font
{
public:

  font() = default;
  font(const std::string &Path, uint Size = 32);
  ~font();

  font(const font &)            = delete;
  font &operator=(const font &) = delete;

  bool load(const std::string &Path, uint Size = 32);
  bool is_loaded() const;

  const glyph &get_glyph(char32_t Codepoint);
  float        get_kerning(char32_t Left, char32_t Right);

  uint  get_size() const;
  float get_ascent() const;
  float get_line_height() const;
  uint  get_texture() const;

private:

  void         unload();
  const glyph &load_glyph(char32_t Codepoint);

  FT_FaceRec_ *Face_       = nullptr;
  uint         Size_       = 0;
  float        Ascent_     = 0.f;
  float        LineHeight_ = 0.f;

  uint     Texture_ = 0;
  iVector2 Cursor_;
  int      RowHeight_ = 0;

  std::unordered_map<char32_t, glyph> Glyphs_;
};
} // namespace marx::graphics
