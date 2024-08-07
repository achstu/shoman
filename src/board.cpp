#include "board.h"
#include "action.h"

#include <bit>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <random>

bool Board::winning() const { return bb_white == 0; }
bool Board::losing() const  { return bb_black == 0; }
bool Board::terminal() const { return winning() || losing(); }

bool Board::black(int i) const    { return bb_black & (1<<i); }
bool Board::white(int i) const    { return bb_white & (1<<i); }
bool Board::occupied(int i) const { return black(i) || white(i); }
bool Board::empty(int i) const    { return !occupied(i); }

int Board::count_black() const { return std::popcount(bb_black); }
int Board::count_white() const { return std::popcount(bb_white); }

uint16_t random_mask() {
  std::mt19937 rng(std::random_device {}());
  std::uniform_int_distribution<> distrib(
    std::numeric_limits<uint16_t>::min(),
    std::numeric_limits<uint16_t>::max()
  );
  uint16_t mask = distrib(rng);
  while (std::popcount(mask) > 4) mask = distrib(rng);
  return mask;
}

Board Board::random() {
  Board board;

  board.bb_black = random_mask();
  board.bb_white = random_mask();
  uint16_t common = board.bb_black & board.bb_white;
  board.bb_black ^= common;
  board.bb_white ^= common;
  return board;
}

Board::Board() : bb_black(0), bb_white(0) {}

Board Board::from_string(std::string board_string) {
  Board board;
  // black = white = 0;
  for (int i = 0; char stone : board_string) {
    switch (stone) {
      case 'b': board.bb_black |= (1<<i); break;
      case 'w': board.bb_white |= (1<<i); break;
      default: break;
    }
    i++;
  }
  return board;
}

std::string Board::to_string() const {
  std::string board_string;
  for (int i = 0; i < 16; i++) {
    if (black(i)) board_string.push_back('b');
    else if (white(i)) board_string.push_back('w');
    else if (empty(i)) board_string.push_back('_');
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

  // Check if a move is a capture move
/*  bool isCaptureMove(const Move& move) {
    // Check if the second move results in capturing an opponent's stone
    Position pos = move.secondMove;
    if (pos.row < 0 || pos.row >= BOARD_SIZE || pos.col < 0 || pos.col >= BOARD_SIZE) {
      return false; // Move is out of bounds
    }
      return boards[pos.board][pos.row][pos.col] != EMPTY && boards[pos.board][pos.row][pos.col] != (currentPlayer == PLAYER_BLACK ? BLACK : WHITE);
  }*/

std::vector<Action> Board::passive_actions() const {
  std::vector<Action> actions;
  for (int i = 0; i < 16; i++) {
    if (black(i)) {
      for (Vector v : Vector::all()) {
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
 

uint32_t Board::to_binary() const {
  return ((uint32_t)bb_black << 16) | bb_white;
}


// #include <cmath>
//Points for each position
const float CENTER_CONTROL = 2.0 / 5.0;
const float EDGE_CONTROL = 1.0 / 5.0;
const float STONE_COUNT = 1.0;
const float MOBILITY = 4.0/5.0;
//const float THREAT = 4.0/5.0;

float Board::evaluate() const {
  if (terminal()) {
    return winning() ? INFINITY : -INFINITY;
  }
  float score = 0;
  // masks
  const uint16_t centerMask = 0b0000011001100000;
  const uint16_t edgeMask = 0b0110100110010110;

  // Center and edge control scoring
  score += CENTER_CONTROL * std::popcount(uint16_t(bb_black & centerMask));
  score += EDGE_CONTROL * std::popcount(uint16_t(bb_black & edgeMask));

  score -= CENTER_CONTROL * std::popcount(uint16_t(bb_white & centerMask));
  score -= EDGE_CONTROL * std::popcount(uint16_t(bb_white & edgeMask));

  // Stone count bonus
  int playerStones = count_black();
  int opponentStones = count_white();
  score += (playerStones - opponentStones) * STONE_COUNT;

  score += mobility_score();
  //score += threat_and_safety_score();

  return score;
}

float Board::mobility_score() const {
  int black_mobility = passive_actions().size();
  Board other = *this;
  other.flip();
  // Temporarily flip to evaluate white's mobility
  int white_mobility = other.passive_actions().size();

  return (black_mobility - white_mobility) * MOBILITY;
}
