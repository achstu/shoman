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
