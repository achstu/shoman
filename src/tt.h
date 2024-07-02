#pragma once


#include "state.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>

// Transposition Table with open addressing
// and linear porbing
struct TT {
  using Key = State;
  using Value = struct {
    float score;
    int depth;
  };

  struct Elem {
    Key key;
    Value value;
  };
  
  // static constexpr size_t SIZE = 0x3B800001;
  static constexpr size_t SIZE = 1000000;
  std::array<Elem, SIZE> map;
  // std::vector<Elem> map;
  
  static size_t probe(size_t x);

  TT() = default;
  bool contains(Key k);
  Value& get(Key k);
  Key& get_key(Key k);
  void insert(Key k, Value v);
};
