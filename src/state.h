#pragma once

#include <string>
#include <vector>

#include "tile.h"
#include "move.h"

struct State {
  Tile tiles[4];
  char player;

  
  State(std::string state_string);
  std::vector<Move> all_moves() const;
  bool apply(Move move);
  std::string to_string() const;
};
