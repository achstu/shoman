#pragma once

#include <string>
#include <vector>

#include "board.h"
#include "move.h"


struct State {
  Board boards[4];

  // constructor
  State(std::string state_string);
  std::string to_string() const;

  // move generation
  std::vector<Move> all_moves() const;

  // state status
  bool terminal() const;
  bool winning() const;
  bool losing() const;

  // making moves
  void make(Move move);
  void flip();

  // comparing states
  bool operator==(const State&) const = default;
  std::size_t hash() const;

  // evaluation funtion
  float evaluate() const;
};
