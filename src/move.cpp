#include "move.h"

#include <map>

std::string extract_uppercase(const std::string& str) {
  std::string result;
  for (const char& c : str) {
    if (isupper(c)) result.push_back(c);
  }
  return result;
}

int Move::length() const {
  if (notation.front() == '2') return 2;
  else return 1;
}

std::string State::vector(const int size) const {
  // TODO: should I do it faster?
  assert(size == 6);
  static const std::map<std::string, int> translate = {
    {"L", -1}, {"R", +1},
    {"U", -6}, {"D", +6},
    {"UR", -5}, {"DL", +5},
    {"UL", -7}, {"DR", +7}
  };
  return translate.at(extract_uppercase(notation));
}



