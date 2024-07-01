#include "search.h"



// simple minimax approach
float minimax(const State& state, int depth = 2) {
  if (depth == 0) {
    return state.evaluate();
  }

  // TODO: std::numeric_limits<float>::inifinity();
  float value = -INFINITY;
  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);

    value = std::max(
      value,
      -minimax(child, depth - 1)
    );
  }
  return value;
}

// simple alpha-beta approach
float 
