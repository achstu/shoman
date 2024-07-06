#pragma once

#include <string>
#include <array>

struct Vector {
  int length;
  int shift;

  std::string to_string() const;

  static constexpr std::array<Vector, 16> all();
};
