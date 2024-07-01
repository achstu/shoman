#include "board.h"

#include <bit>
#include <cstdint>

bool Board::winning() const { return bb_white == 0; }
bool Board::losing() const  { return bb_black == 0; }
bool Board::terminal() const { return winning() || losing(); }

bool Board::black(int i) const    { return bb_black & (1<<i); }
bool Board::white(int i) const    { return bb_white & (1<<i); }
bool Board::occupied(int i) const { return black(i) || white(i); }
bool Board::empty(int i) const    { return !occupied(i); }

int Board::count_black() const { return std::popcount(bb_black); }
int Board::count_white() const { return std::popcount(bb_white); }

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
std::string Board::to_string() const {
  std::string board_string;
  for (int i = 0; i < 16; i++) {
    if (black(i)) board_string.push_back('b');
    if (white(i)) board_string.push_back('w');
    if (empty(i)) board_string.push_back('_');
  }
  return board_string;
}

// manhatan distance
int d(int x, int y) { return abs(x - y); }

// checks if push from a to b is valid
static bool valid_push(int a, int b) {
  // t / 4 -> row
  // t % 4 -> col
  return 0 <= b && b < 16 && (abs(a/4 - b/4) | abs(a%4 - b%4)) < 3;
}

bool Board::valid_passive_action(Action action) const {
  return
    valid_push(action.start(), action.end()) &&
    !occupied(action.mid()) &&
    !occupied(action.end());
}

bool Board::valid_aggressive_action(Action action) const {
  bool opponent_push =
    valid_push(action.start(), action.end()) &&
    white(action.mid()) + white(action.end()) == 1 &&
    black(action.mid()) + black(action.end()) == 0 &&
    (empty(action.after()) || !valid_push(action.end(), action.after()));
  return valid_passive_action(action) || opponent_push;
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

void Board::flip() { std::swap(bb_black, bb_white); }

void push_stone(uint16_t &bb, int from, int to) {
  bb ^= (1 << from);
  if (valid_push(from, to))
    bb ^= (1 << to);
}

void Board::make(Action action) {
  // firstly remove stone from starting position
  push_stone(bb_black, action.start(), action.end());
  if (white(action.mid())) push_stone(bb_white, action.mid(), action.after());
  if (white(action.end())) push_stone(bb_white, action.end(), action.after());

}
 

std::size_t Board::hash() const {
  return ((std::size_t)bb_black << 16) | bb_white;
}

#include <cmath>

float Board::evaluate() const {
  if (terminal()) {
    return winning() ? INFINITY : -INFINITY;
  }
  float score = 0;
  // masks
  const uint16_t centerMask = 0b0000011001100000;
  const uint16_t edgeMask = 0b0110100110010110;

  //Points for each position
  const float CENTER_CONTROL = 2.0 / 5.0;
  const float EDGE_CONTROL = 1.0 / 5.0;
  const float STONE_COUNT = 1.0;


  // Center and edge control scoring
  score += CENTER_CONTROL * std::popcount(uint16_t(bb_black & centerMask));
  score += EDGE_CONTROL * std::popcount(uint16_t(bb_black & edgeMask));

  score -= CENTER_CONTROL * std::popcount(uint16_t(bb_white & centerMask));
  score -= EDGE_CONTROL * std::popcount(uint16_t(bb_white & edgeMask));

  // Stone count bonus
  int playerStones = count_black();
  int opponentStones = count_white();
  score += (playerStones - opponentStones) * STONE_COUNT;

  return score;
}
