#include "search.h"


#include <cmath>

// simple minimax approach
float minimax(const State& state, int depth) {
  if (depth == 0) {
    return state.evaluate();
  }

  // TODO: std::numeric_limits<float>::inifinity();
  float score = -INFINITY;
  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);

    score = std::max(
      score,
      -minimax(child, depth - 1)
    );
  }
  return score;
}

// simple alpha-beta approach
// alpha := minimum score that the maximizing player is assured of
// beta := maximum score that the minimizing player is assured of
float alpha_beta(float alpha, float beta, const State& state, int depth) {
  if (depth == 0) {
    return state.evaluate();
  }

  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move); 
  }
  return 1.0;  
} 
