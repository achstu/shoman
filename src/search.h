#pragma once

#include "state.h"

float search(const State& state, int depth);

float minimax(const State& state, int depth);

float alphabeta(const State& state, int depth, float alpha, float beta);

