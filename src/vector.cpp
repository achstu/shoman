#include "vector.h"

#include <format>
#include <map>


std::string Vector::to_string() const {
  // TODO: trans is mapping from int -> string
  // can be done faster
  static const std::map<int, std::string> trans = {
    {-1, "L"}, {+1, "R"},
    {-4, "U"}, {+4, "D"},
    {-3, "UR"}, {+3, "DL"},
    {-5, "UL"}, {+5, "DR"}
  };
  return std::format("{}{}", length, trans.at(shift));
}

std::array<Vector, 16> Vector::all() {
  // TODO: brace-enclosed initializer list
  return {
    // L and R
    Vector{1, -1}, {2, -1},
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
  };
}
