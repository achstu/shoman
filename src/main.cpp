#include <iostream>
#include <string>
#include <vector>

#include "move.h"
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

char other(char player) { return (player == 'b') ? 'w' : 'b'; }

const int SZ = 998'244'353;
bool hash_table[SZ];

bool& visited(const State& state) {
  return hash_table[state.hash() % SZ];
}



int move_count(const State& state, char player, int depth) {
  if (depth == 0) return 1;

  
  
}

void move_search(const State& state, char player, int depth = 0) {

  auto moves = state.all_moves();
  for (Move move : moves) {
    State child = state;
    child.make(move);

  }
}

int main() {
  std::string state_string;
  getline(std::cin, state_string);
  State state(state_string);
  move_search(state, 'b');
  // std::cout << random_move(state).to_string() << std::endl;
}

