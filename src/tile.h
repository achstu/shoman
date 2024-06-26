#pragma once

#include <string>

struct Tile {
  char b[6 * 6];

  Tile(std::string setting);
  std::string to_string() const;
  
};
