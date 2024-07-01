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

float alpha_beta_min(float alpha, float beta, const State& state, int depth);

float alpha_beta_max(float alpha, float beta, const State& state, int depth) {
  if (depth == 0 || state.terminal()) return state.evaluate();
  
  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);
    
    float score = alpha_beta_min(alpha, beta, child, depth-1);
    if(score >= beta) return beta;   // fail hard beta-cutoff
    if(score > alpha) alpha = score; // alpha acts like max in MiniMax
  }
  return alpha;
}

float alpha_beta_min(float alpha, float beta, const State& state, int depth) {
  if (depth == 0 || state.terminal()) return -state.evaluate();
  
  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);
    
    float score = alpha_beta_max(alpha, beta, child, depth-1);
    if (score <= alpha) return alpha; // fail hard alpha-cutoff
    if (score < beta) beta = score;   // beta acts like min in MiniMax
   }
   return beta;
}

float alpha_beta(const State& state, int depth) {
  return alpha_beta_max(-INFINITY, INFINITY, state, depth);
} 
