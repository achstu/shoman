#include "tt.h"
#include <cstddef>


size_t TT::probe(size_t x) {
  // gcd(a, SIZE) = 1
  constexpr size_t a = 85654861; // 0x51AFD4D
  constexpr size_t b = 2003; // b is obsolete
  return a * x + b;
}

#include <iostream>

TT::Value& TT::get(Key k) {
  return map[k.hash() % SIZE].value;
}

TT::Key& TT::get_key(Key k) {
  return map[k.hash() % SIZE].key;
}

bool TT::contains(Key k) {
  return get(k).depth != 0;
} 

void TT::insert(Key k, Value v) {
  map[k.hash() % SIZE] = {k, v};
}
