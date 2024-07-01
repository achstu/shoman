#pragma once

#include "state.h"

float minimax(const State& state, int depth);

float alphabeta(float alpha, float beta, const State& state, int depth);

