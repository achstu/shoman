#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "action.h"


struct Board {
  uint16_t bb_black;
  uint16_t bb_white;

  // constructors
  Board();
  static Board random();
  Board(std::string board_string);
  std::string to_string() const;

  // board status
  bool winning() const;
  bool losing() const;
  bool terminal() const;
  
  // info about stones
  bool black(int i) const;
  bool white(int i) const;
  bool occupied(int i) const;
  bool empty(int i) const;

  int count_black() const;
  int count_white() const;
  
  // move generation
  bool valid_passive_action(Action action) const;
  bool valid_aggressive_action(Action action) const;
  std::vector<Action> passive_actions() const;
  std::vector<Action> aggressive_actions(Vector vector) const;

  // making moves
  void make(Action action);
  void flip();

  // comparing boards
  bool operator==(const Board&) const = default;
  uint32_t to_binary() const;

  // evaluation function
  float evaluate() const;
  float mobility_score() const;
};
