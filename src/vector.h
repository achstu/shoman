#pragma once

#include <string>
#include <array>

struct Vector {
  int length;
  int shift;

  std::string to_string() const;

  // TODO: make it constexpr somehow
  static std::array<Vector, 16> all();
};
