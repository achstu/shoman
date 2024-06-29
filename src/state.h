#pragma once

#include <string>
#include <vector>

#include "board.h"
#include "move.h"


// assuming that its black turn
struct State {
  Board boards[4];

  // std::vector<Board> homeboards() const { return {boards[0], boards[1]}; };
  // std::vector<
  
  // constructor
  State(std::string state_string);
  // std::string to_string() const;

  // move generation
  std::vector<Move> all_moves() const;

  // state validation
  bool terminal() const;
  bool winning() const;
  bool losing() const;
  
  bool valid_move(Move move) const;
  void make(Move move);
};
