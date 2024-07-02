#pragma once


#include <cstdint>

using hash_t = uint32_t;

namespace hash {
 
  // 32-bit FNV-1a hash
  hash_t fnv1a(const __uint128_t value);


  // 32-bit FNV-1a hash function
  // hash_t fnv1a(const uint8_t* bytes, std::size_t count);

  // 32-bit FNV-1 hash function
  // hash_t fnv1(const uint32_t* bytes, std::size_t count);
}
