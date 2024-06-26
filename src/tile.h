#pragma once

#include <string>

struct Tile {
  char b[16];

  Tile(std::string setting);
  void print() const;
};
