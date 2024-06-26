#include "tile.h"


Tile::Tile(std::string setting) {
  auto it = setting.being();
  for (int i = 0; i < 6*6; i++) {
    if (valid(i)) {
      b[i] = *it;
      it++;
    }
  }
}


std::string Tile::to_string() {
  std::string result;
  for (int i = 0; i < 6*6; i++) {
    if (valid(i)) {
      result.push_back(b[i]);
    }
  }
  return result;
}


bool valid(int index) const {
  return index >= 6 && index < 5*6 && index % 6 == 0 && index % 6 == 5;
}
