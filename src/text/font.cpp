#include "marx/graphics-internal/text/font.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <freetype/freetype.h>
#include <iostream>
#include <string>
#include <vector>

#include "marx/graphics-internal/vector.hpp"
#include "marx/graphics-internal/window.hpp"

namespace
{
constexpr int AtlasSize    = 1024;
constexpr int AtlasPadding = 1;

FT_Library Library_ = nullptr;
int        Users_   = 0;

bool acquire_library()
{
  if (Library_)
  {
    ++Users_;
    return true;
  }

  if (FT_Init_FreeType(&Library_))
  {
    std::cerr << "Failed to initialize FreeType.\n";
    Library_ = nullptr;
    return false;
  }

  Users_ = 1;
  return true;
}

void release_library()
{
  if (--Users_ > 0)
    return;

  FT_Done_FreeType(Library_);
  Library_ = nullptr;
  Users_   = 0;
}
} // namespace

namespace marx::graphics
{
font::font(const std::string &Path, uint Size)
{
  load(Path, Size);
}

font::~font()
{
  unload();
}

bool font::load(const std::string &Path, uint Size)
{
  unload();

  if (!get_display())
  {
    std::cerr << "A window must exist before a font can be loaded.\n";
    return false;
  }

  if (!acquire_library())
    return false;

  if (FT_New_Face(Library_, Path.c_str(), 0, &Face_))
  {
    std::cerr << "Failed to load font: " << Path << '\n';
    Face_ = nullptr;
    release_library();
    return false;
  }

  if (FT_Set_Pixel_Sizes(Face_, 0, Size))
  {
    std::cerr << "Failed to set font size: " << Size << '\n';
    unload();
    return false;
  }

  Size_       = Size;
  Ascent_     = std::round(Face_->size->metrics.ascender / 64.f);
  LineHeight_ = std::round(Face_->size->metrics.height / 64.f);

  std::vector<unsigned char> Empty(static_cast<std::size_t>(AtlasSize) *
                                   AtlasSize);

  glGenTextures(1, &Texture_);
  glBindTexture(GL_TEXTURE_2D, Texture_);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, AtlasSize, AtlasSize, 0, GL_RED,
               GL_UNSIGNED_BYTE, Empty.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}

bool font::is_loaded() const
{
  return Face_ != nullptr;
}

const glyph &font::get_glyph(char32_t Codepoint)
{
  auto Found = Glyphs_.find(Codepoint);

  if (Found != Glyphs_.end())
    return Found->second;

  return load_glyph(Codepoint);
}

float font::get_kerning(char32_t Left, char32_t Right)
{
  if (!Face_ || !FT_HAS_KERNING(Face_))
    return 0.f;

  FT_Vector Delta;

  FT_Error Error = FT_Get_Kerning(Face_, FT_Get_Char_Index(Face_, Left),
                                  FT_Get_Char_Index(Face_, Right),
                                  FT_KERNING_DEFAULT, &Delta);

  if (Error)
    return 0.f;

  return Delta.x / 64.f;
}

uint font::get_size() const
{
  return Size_;
}

float font::get_ascent() const
{
  return Ascent_;
}

float font::get_line_height() const
{
  return LineHeight_;
}

uint font::get_texture() const
{
  return Texture_;
}

void font::unload()
{
  if (Texture_ && get_display())
    glDeleteTextures(1, &Texture_);

  if (Face_)
  {
    FT_Done_Face(Face_);
    release_library();
  }

  Face_       = nullptr;
  Size_       = 0;
  Ascent_     = 0.f;
  LineHeight_ = 0.f;

  Texture_   = 0;
  Cursor_    = { AtlasPadding, AtlasPadding };
  RowHeight_ = 0;

  Glyphs_.clear();
}

const glyph &font::load_glyph(char32_t Codepoint)
{
  glyph &Glyph = Glyphs_[Codepoint];

  if (!Face_ ||
      FT_Load_Char(Face_, Codepoint, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP))
    return Glyph;

  const FT_GlyphSlot Slot   = Face_->glyph;
  const FT_Bitmap   &Bitmap = Slot->bitmap;

  Glyph.Advance = Slot->advance.x / 64.f;

  if (Bitmap.width == 0 || Bitmap.rows == 0 ||
      Bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
    return Glyph;

  const int Width  = static_cast<int>(Bitmap.width);
  const int Height = static_cast<int>(Bitmap.rows);

  if (Cursor_.x + Width + AtlasPadding > AtlasSize)
  {
    Cursor_.x = AtlasPadding;
    Cursor_.y += RowHeight_ + AtlasPadding;
    RowHeight_ = 0;
  }

  if (Cursor_.y + Height + AtlasPadding > AtlasSize)
  {
    std::cerr << "Font atlas is full.\n";
    return Glyph;
  }

  std::vector<unsigned char> Pixels(static_cast<std::size_t>(Width) * Height);

  for (int Row = 0; Row < Height; ++Row)
  {
    const unsigned char *Source = Bitmap.buffer + Row * Bitmap.pitch;
    std::copy(Source, Source + Width, Pixels.begin() + Row * Width);
  }

  glBindTexture(GL_TEXTURE_2D, Texture_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0, Cursor_.x, Cursor_.y, Width, Height, GL_RED,
                  GL_UNSIGNED_BYTE, Pixels.data());
  glBindTexture(GL_TEXTURE_2D, 0);

  const float Scale = 1.f / AtlasSize;

  Glyph.Size    = { static_cast<float>(Width), static_cast<float>(Height) };
  Glyph.Bearing = { static_cast<float>(Slot->bitmap_left),
                    static_cast<float>(Slot->bitmap_top) };
  Glyph.UvMin   = { Cursor_.x * Scale, Cursor_.y * Scale };
  Glyph.UvMax   = { (Cursor_.x + Width) * Scale, (Cursor_.y + Height) * Scale };

  Cursor_.x += Width + AtlasPadding;
  RowHeight_ = std::max(RowHeight_, Height);

  return Glyph;
}
} // namespace marx::graphics
