#pragma once

#include <string>

enum Tile {
  EMPTY,
  BLACK,
  WHITE,
  VOID
};


struct Board {
  static const int SIZE = 6 * 6;
  Tile tiles[SIZE];

  // from_string() method
  Board(std::string board_string);
  // not nessesery
  // std::string to_string() const;

  // move generation
  bool occupied(int index) const;
  std::vector<Action> passive_actions(char player) const;
  std::vector<Action> aggresive_moves(int vector) const;
  

// private: // ???
  // static bool valid_index(int index);
  
};
