#pragma once


#include "state.h"



// Transposition Table with open addressing
// and linear porbing
struct TT {
  struct Value {
    float score;
    int depth;
  };
  using Key = const State&;

  static constexpr std::size_t M = 0x3B800001;
  Value* map[M];

  TT();
  // alpha, alpha < threshold
  
  static std::size_t probe(Key k, std::size_t x);

  
  void insert(Key k, Value v);
  void search(Key k);
};
