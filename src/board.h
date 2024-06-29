#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "action.h"


// bitboard iml
// always assuming that its black's move
struct Board {
  uint16_t bb_black;
  uint16_t bb_white;

  // from_string() method
  Board();
  Board(std::string board_string);
  // std::string to_string() const;

  // winnig and losing boards
  bool winning() const;
  bool losing() const;

  // board status
  bool black(int i) const;
  bool white(int i) const;
  bool occupied(int i) const;
  bool empty(int i) const;

  // move generation
  bool valid_passive_action(Action action) const;
  bool valid_aggressive_action(Action action) const;
  std::vector<Action> passive_actions() const;
  std::vector<Action> aggressive_actions(Vector vector) const;

  // making moves
  void make(Action action);
  void unmake(Action action);
};
