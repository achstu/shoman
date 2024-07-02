#include "search.h"
#include "state.h"


#include <limits>

#define INF std::numeric_limits<float>::infinity()

// simple minimax approach
float minimax(const State& state, int depth) {
  if (depth == 0 || state.terminal()) {
    return state.evaluate();
  }

  // TODO: std::numeric_limits<float>::inifinity();
  float score = -INF;
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
float alphabeta(const State& state, int depth, float alpha, float beta) {
  if (depth == 0 || state.terminal()) {
    return state.evaluate();
  }

  float score = -INF;
  for (const Move& move : state.all_moves()) {
    State child = state;
    child.make(move);

    score = std::max(score, -alphabeta(child, depth-1, -beta, -alpha));
    alpha = std::max(alpha, score);
    if (alpha >= beta) {
      break; // Beta cut-off
    }
  }
  return score;
}

float search(const State& state, int depth) {
  return alphabeta(state, depth, -INF, INF);
}
