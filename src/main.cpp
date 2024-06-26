#include <iostream>
#include <string>
#include <vector>

#include "state.h"


// function that for given state returns evaluation on scale (-10, 10)
// as value smaller then move better for black player
int minimax(const State& state, int depth = 0) {
  const static int MAX_DEPTH = 20;

  if (depth >= MAX_DEPTH) {
    return state.evaluate();
  }

  int value = 0;
  for (Move& move : state.all_moves()) {
    State child = state;
    child.apply(move);
    minimax(child, depth + 1);
  }
  return value;
}

int main() {
  std::string setting;
  getline(std::cin, setting);

  State state(setting);
  
}

