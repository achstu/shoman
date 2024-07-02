#pragma once

#include <string>

#include "action.h"
#include "vector.h"


struct Move {
  Vector vector;
  int passive_board;
  int passive_index;
  int aggressive_board;
  int aggressive_index;

  Move() = default;
  Move(int i, int j, Action passive, Action aggressive);
  std::string to_string() const;

};
