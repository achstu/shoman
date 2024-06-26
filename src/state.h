#pragma once

#include <string>
#include <vector>

#include "tile.h"
#include "move.h"

struct State {
  Tile tiles[4];
  char player;

  
  State(std::string setting);
  std::vector<Move> all_moves() const;
  void apply(Move move);
  int evaluate() const;
  void print() const;
};
