#include "hash.h"


#define FNV_32_PRIME        0x01000193
#define FNV_32_OFFSET_BASIS 0x811c9dc5

hash_t hash::fnv1a(__uint128_t value) {
  hash_t hash = FNV_32_OFFSET_BASIS;

  for (std::size_t i = 0; i < sizeof(value); i++) {
    uint8_t byte = static_cast<uint8_t>(value);

    hash ^= byte;
    hash *= FNV_32_PRIME;
    
    value >>= 8;
  }
  return hash;  
}

