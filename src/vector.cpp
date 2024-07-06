#include "vector.h"

#include <format>
#include <array>

std::string Vector::to_string() const {
  static constexpr std::array<std::pair<int, const char*>, 8> trans = {{
    {-1, "L"}, {+1, "R"},
    {-4, "U"}, {+4, "D"},
    {-3, "UR"}, {+3, "DL"},
    {-5, "UL"}, {+5, "DR"},
  }};
  for (const auto& [key, dir] : trans) {
    if (key == shift) {
      return std::format("{}{}", length, dir);
    }
  }
  return "";  
}

constexpr std::array<Vector, 16> Vector::all() {
  return {{
    // L and R
    {1, -1}, {2, -1},
    {1, +1}, {2, +1},
    // U and D
    {1, -4}, {2, -4},
    {1, +4}, {2, +4},
    // UR and DL
    {1, -3}, {2, -3},
    {1, +3}, {2, +3},
    // UL and DR
    {1, -5}, {2, -5},
    {1, +5}, {2, +5}
  }};
}
