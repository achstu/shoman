#pragma once

#include <cstdint>

namespace hash {

  // FNV-1a hash function
  uint32_t fnv1a_32(const __uint128_t value);
  uint64_t fnv1a_64(const __uint128_t value);

}
