#include "marx/graphics-internal/text/utf8.hpp"

#include <cstddef>
#include <string>

namespace
{
constexpr char32_t ReplacementCharacter = 0xFFFD;

constexpr char32_t MinimumCodepoints[] = { 0, 0, 0x80, 0x800, 0x10000 };

bool is_valid_codepoint(char32_t Codepoint)
{
  return Codepoint <= 0x10FFFF && (Codepoint < 0xD800 || Codepoint > 0xDFFF);
}
} // namespace

namespace marx::graphics::utf8
{
char32_t decode(const std::string &String, std::size_t &Index)
{
  const unsigned char First = String[Index];

  std::size_t Length    = 1;
  char32_t    Codepoint = First;

  if (First >= 0xF8)
  {
    ++Index;
    return ReplacementCharacter;
  }
  else if (First >= 0xF0)
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

  if (Codepoint < MinimumCodepoints[Length] || !is_valid_codepoint(Codepoint))
    return ReplacementCharacter;

  return Codepoint;
}

std::string encode(char32_t Codepoint)
{
  if (!is_valid_codepoint(Codepoint))
    Codepoint = ReplacementCharacter;

  std::string Result;

  if (Codepoint < 0x80)
  {
    Result += static_cast<char>(Codepoint);
  }
  else if (Codepoint < 0x800)
  {
    Result += static_cast<char>(0xC0 | (Codepoint >> 6));
    Result += static_cast<char>(0x80 | (Codepoint & 0x3F));
  }
  else if (Codepoint < 0x10000)
  {
    Result += static_cast<char>(0xE0 | (Codepoint >> 12));
    Result += static_cast<char>(0x80 | ((Codepoint >> 6) & 0x3F));
    Result += static_cast<char>(0x80 | (Codepoint & 0x3F));
  }
  else
  {
    Result += static_cast<char>(0xF0 | (Codepoint >> 18));
    Result += static_cast<char>(0x80 | ((Codepoint >> 12) & 0x3F));
    Result += static_cast<char>(0x80 | ((Codepoint >> 6) & 0x3F));
    Result += static_cast<char>(0x80 | (Codepoint & 0x3F));
  }

  return Result;
}

std::string from_utf16(const std::u16string &String)
{
  std::string Result;

  for (std::size_t Index = 0; Index < String.size(); ++Index)
  {
    char32_t Codepoint = String[Index];

    const bool High = Codepoint >= 0xD800 && Codepoint <= 0xDBFF;
    const bool Low  = Codepoint >= 0xDC00 && Codepoint <= 0xDFFF;

    if (High && Index + 1 < String.size() && String[Index + 1] >= 0xDC00 &&
        String[Index + 1] <= 0xDFFF)
    {
      Codepoint =
          0x10000 + ((Codepoint - 0xD800) << 10) + (String[Index + 1] - 0xDC00);
      ++Index;
    }
    else if (High || Low)
      Codepoint = ReplacementCharacter;

    Result += encode(Codepoint);
  }

  return Result;
}

std::string from_utf32(const std::u32string &String)
{
  std::string Result;

  for (char32_t Codepoint : String)
    Result += encode(Codepoint);

  return Result;
}

std::size_t length(const std::string &String)
{
  std::size_t Count = 0;

  for (std::size_t Index = 0; Index < String.size(); ++Count)
    decode(String, Index);

  return Count;
}

std::size_t byte_index(const std::string &String, std::size_t CodepointIndex)
{
  std::size_t Index = 0;

  for (std::size_t Count = 0; Count < CodepointIndex && Index < String.size();
       ++Count)
    decode(String, Index);

  return Index;
}

void pop_back(std::string &String)
{
  if (String.empty())
    return;

  std::size_t Index = String.size() - 1;

  while (Index > 0 &&
         (static_cast<unsigned char>(String[Index]) & 0xC0) == 0x80)
    --Index;

  String.erase(Index);
}
} // namespace marx::graphics::utf8
