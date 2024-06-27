#include <iostream>
#include <string>
#include <vector>

#include "state.h"


#include <random>
Move random_move(const State& state) {
  auto moves = state.all_moves();
  std::mt19937 rng;
  int index = rng() % moves.size();
  return moves[index];
}

int main() {
  std::string board;
  while (getline(std::cin, board)) {
    State state(board);
    std::cout << random_move(state).to_string() << std::endl;
  }
}

