#include "state.h"

#include <format>
#include <sstream>



// returns false if move is invalid
// applies move otherwise
bool State::apply(Move move) {
  return true;
}



std::string State::to_string() const {
  std::string result = std::format("{}: ", player);
  for (Tile tile : tiles) {
    result.push_back(' ');
    result.append(tile.to_string());
  }
  return result;
}


State::State(std::string setting) {
  std::stringstream stream(setting);
  stream >> player;

  int i = 0;
  std::string tile;
  while (stream >> tile) {
    tile[i] = Tile(tile);
    i++;
  }
}

std::vector<Move> State::all_moves() const {
  return {};
}

int State::evaluate() const {
  return 132;
}
