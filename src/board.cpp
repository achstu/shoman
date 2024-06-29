#include "board.h"
#include "action.h"

bool Board::winning() const { return bb_white == 0; }
bool Board::losing() const  { return bb_black == 0; }

bool Board::black(int i) const    { return bb_black & (1<<i); }
bool Board::white(int i) const    { return bb_white & (1<<i); }
bool Board::occupied(int i) const { return black(i) || white(i); }
bool Board::empty(int i) const    { return !occupied(i); }

Board::Board() : bb_black(0), bb_white(0) {}
Board::Board(std::string board_string) : bb_black(0), bb_white(0) {
  // black = white = 0;
  for (int i = 0; char stone : board_string) {
    switch (stone) {
      case 'b': bb_black |= (1<<i); break;
      case 'w': bb_white |= (1<<i); break;
      default: break;
    }
    i++;
  }
}

// manhatan distance
int d(int x, int y) { return abs(x - y); }

// checks if jump from a to b is valid
static bool valid_jump(int a, int b) {
  // t / 4 -> row
  // t % 4 -> col
  return 0 <= b && b < 16 && (abs(a/4 - b/4) | abs(a%4 - b%4)) < 3;
}

bool Board::valid_passive_action(Action action) const {
  return valid_jump(action.index, action.end()) && !occupied(action.mid()) && !occupied(action.end());
}

bool Board::valid_aggressive_action(Action action) const {
  bool valid_push = (white(action.mid()) + white(action.end()) == 1) && (black(action.mid()) + black(action.end()) == 0) && empty(action.after());
  return valid_passive_action(action) || valid_push;
}



std::vector<Action> Board::passive_actions() const {
  std::vector<Action> actions;
  for (int i = 0; i < 16; i++) {
    if (black(i)) {
      for (Vector v : vectors) {
        Action action = {
          .index = i,
          .vector = v
        };
        if (valid_passive_action(action)) {
          actions.push_back(action);
        }
      }
    }
  }
  return actions;
}

std::vector<Action> Board::aggressive_actions(Vector vector) const {
  std::vector<Action> actions;
  for (int i = 0; i < 16; i++) {
    if (black(i)) {
      Action action = {
        .index = i,
        .vector = vector
      };
      if (valid_aggressive_action(action)) {
        actions.push_back(action);
      }
    }
  }
  return actions;
}

// assuming that action is valid
// passive move implemented
void Board::make(Action action) {
  // auto [i, d, v] = action;
  // bb_black ^= (1 << action.index);
  // bb_black ^= (1 << action.index + action.lenght * action.vector);

  std::swap(bb_black, bb_white);
}

void Board::unmake(Action action) {
  make(action);
}
