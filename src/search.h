#pragma once

#include "state.h"
#include "tt.h"

#include <limits>

#define INF (std::numeric_limits<float>::infinity())


// current best searching algorithm
// float search(const State& state, int depth);

float minimax(const State& state, int depth);
float alphabeta(const State& state, int depth, float alpha = INF, float beta = -INF);
float tt_alphabeta(TranspositionTable& tt, const State& state, int depth, float alpha = -INF, float beta = INF);
