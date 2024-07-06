#include "hash.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>

template <typename T, T FNV_OFFSET_BASIS, T FNV_PRIME>
T fnv1a(__uint128_t value) {
  static_assert(std::is_integral<T>::value);

  T hash = FNV_OFFSET_BASIS;

  for (std::size_t i = 0; i < sizeof(value); i++) {
    uint8_t byte = static_cast<uint8_t>(value);

    hash ^= byte;
    hash *= FNV_PRIME;

    value >>= 8;
  }
  return hash;
}

#define FNV_32_PRIME 0x01000193
#define FNV_32_OFFSET_BASIS 0x811c9dc5

uint32_t hash::fnv1a_32(__uint128_t value) {
  return fnv1a<uint32_t, FNV_32_OFFSET_BASIS, FNV_32_PRIME>(value);
}

#define FNV_64_PRIME 0x00000100000001B3
#define FNV_64_OFFSET_BASIS 0xcbf29ce484222325

uint64_t hash::fnv1a_64(__uint128_t value) {
  return fnv1a<uint64_t, FNV_64_OFFSET_BASIS, FNV_64_PRIME>(value);
}
