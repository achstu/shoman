#include "tt.h"
#include <cstddef>
#include <stdexcept>

size_t TranspositionTable::probe(size_t x) {
  // TODO: consider better probing function
  // linear probing implementation
  // gcd(a, SIZE) = 1
  constexpr size_t a = 85654861; // 0x51AFD4D
  constexpr size_t b = 2003; // b is obsolete
  return (a * x + b) % SIZE;
}



bool TranspositionTable::occupied(size_t pos) const {
  return table[pos].value.depth > 0;
}

// @returns index of Key k in table
// if table does not contain k then suitable index
// is returned
size_t TranspositionTable::find(Key k) const {
  size_t pos = k.hash() % SIZE;
  while (occupied(pos) && table[pos].key != k) {
    pos = probe(pos);
  }
  return pos;
}

bool TranspositionTable::contains(Key k) const {
  return occupied(find(k));
}

void TranspositionTable::update(Key k, Value v) {
  table[find(k)] = {k, v};  
}


#include <iostream>
TranspositionTable::Value& TranspositionTable::operator[](Key k) {
  if (!contains(k)) {
    std::cout << k.to_string() << std::endl;
    throw std::runtime_error("Key not found");
  }
  
  size_t pos = k.hash() % SIZE;
  while (table[pos].key != k) {
    pos = probe(pos);
  }
  return table[pos].value;
}
