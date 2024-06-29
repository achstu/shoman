#include <iostream>
#include <string>
#include <vector>

#include "state.h"

#include <format>
#include <random>

Move random_move(const State& state) {
  auto moves = state.all_moves();

  std::cout << std::format("Possible moves: {}\n", moves.size());
  
  std::mt19937 rng(0);
  std::uniform_int_distribution<> distrib(0, moves.size() - 1);
  return moves[distrib(rng)];
}

int main() {
  std::string board_string;
  getline(std::cin, board_string);
  State state(board_string);
  std::cout << random_move(state).to_string() << std::endl;
}

