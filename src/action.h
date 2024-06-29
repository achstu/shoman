#pragma once

#include <string>

#include "vector.h"

struct Action {
  int index;
  Vector vector;

  std::string to_string() const;

  int mid() { return index + vector.shift; }
  int end() { return index + vector.length * vector.shift; }
  int after() { return end() + vector.shift; }
};
