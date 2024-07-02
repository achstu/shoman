#include "tt.h"
#include <cstddef>


std::size_t TT::probe(Key k, std::size_t x) {
  // gcd(a, M) = 1
  constexpr size_t a = 85654861; // 0x51AFD4D
  return a * x;
}

void TT::insert(Key k, Value v) {
  size_t x = 1;
  size_t hash = k.hash();
  size_t index = hash;

  while (map[index] != nullptr) {
    index = (hash + probe(k, x)) % M;
    x = x + 1;
  }

  map[index] = &v;
}





