#pragma once

#include <string>
#include <vector>

#include "board.h"
#include "move.h"
#include "hash.h"


struct State {
  Board boards[4];

  // constructor
  State() = default;
  
  static State from_string(std::string state_string);
  static State random();

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
  __uint128_t to_binary() const;
  uint64_t hash() const;

  // evaluation funtion
  float evaluate() const;
};
