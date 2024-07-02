#include "search.h"
#include "state.h"
#include "tt.h"

// simple minimax approach
float minimax(const State& state, int depth) {
  if (depth == 0 || state.terminal()) {
    return state.evaluate();
  }

  float score = -INF;
  for (const Move& move : state.all_moves()) {
    State child = state;
    child.make(move);

    score = std::max(
      score,
      -minimax(child, depth - 1)
    );
  }
  return score;
}

// alpha-beta approach
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

// alpha-beta with transposition table
float tt_alphabeta(TranspositionTable& tt, const State& state, int depth,
                   float alpha, float beta) {
  if (depth == 0 || state.terminal()) {
    return state.evaluate();
  }
  if (tt.contains(state) && tt[state].depth >= depth) {
    return tt[state].score;
  }

  float score = -INF;
  Move best;
  for (const Move& move : state.all_moves()) {
    State child = state;
    child.make(move);

    float child_score = -tt_alphabeta(tt, child, depth-1, -beta, -alpha);
    
    // score = std::max(score, child_score);
    if (score < child_score) {
      score = child_score;
      best = move;
    }
    
    alpha = std::max(alpha, score);
    if (alpha >= beta) {
      break; // Beta cut-off
    }
  }

  tt.update(state, {score, depth, best});
  return score;
}
