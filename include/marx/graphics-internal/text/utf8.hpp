#pragma once

#include <cstddef>
#include <string>

namespace marx::graphics::utf8
{
char32_t    decode(const std::string &String, std::size_t &Index);
std::string encode(char32_t Codepoint);

std::string from_utf16(const std::u16string &String);
std::string from_utf32(const std::u32string &String);

std::size_t length(const std::string &String);
std::size_t byte_index(const std::string &String, std::size_t CodepointIndex);
void        pop_back(std::string &String);
} // namespace marx::graphics::utf8
