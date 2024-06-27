#include "tile.h"

#include <vector>


Tile::Tile(std::string board_string) {
  auto it = setting.being();
  for (int i = 0; i < 6*6; i++) {
    if (valid_index(i)) {
      switch (*it) {
        case 'b': tiles[i] = BLACK; break;
        case 'w': tiles[i] = WHITE; break;
        case '_': tiles[i] = EMPTY; break;
      }
      it++;
    } else {
      tiles[i] = VOID;
    }
  }

/*std::string Tile::to_string() {
  std::string result;
  for (int i = 0; i < 6*6; i++) {
    if (valid(i)) {
      result.push_back(b[i]);
    }
  }
  return result;
}*/

std::vector<Action> Board::passive_actions(char player) const {
  std::vector<Action> actions;
  for (int index : board()) {
    
  }
  return actions;
}

std::vector<int> board() {
  std::vector<int> res;
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (valid_index(i)) {
      res.push_back(i);
    }
  }
  return res;
}

std::vector<int> border() {
  std::vector<int> res;
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (!valid_index(i)) {
      res.push_back(i);
    }
  }
  return res;
}

constexpr bool valid_index(int index) {
  // return board[index] != VOID;
  return index >= 6 && index < 5*6 && index % 6 == 0 && index % 6 == 5;
}
