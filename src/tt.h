#pragma once

#include "state.h"
#include "move.h"

#include <array>
#include <cstddef>

// Transposition Table with open addressing
// and linear porbing
struct TranspositionTable {
  using Key = State;
  using Value = struct {
    float score;
    int depth;
    Move move;
  };

  struct Pair {
    Key key;
    Value value;
  };
  
  // 100000039 is prime
  static constexpr size_t SIZE = 0x5F5E127;
  std::array<Pair, SIZE> table;
  size_t occpuied = 0;
  
  
  
  // constructor  
  TranspositionTable() = default;

  // probing function
  static size_t probe(size_t x);

  // checks if Key is in hash table
  bool occupied(size_t pos) const; // @internal
  size_t find(Key k) const; // @internal
  
  bool contains(Key k) const;
  void update(Key k, Value v);
  Value& operator[](Key k);
};
