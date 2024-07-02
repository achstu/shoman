#include "state.h"
#include "board.h"
#include "hash.h"

#include <sstream>

#include <algorithm>
#include <ranges>

bool State::terminal() const { return winning() || losing(); }
bool State::winning() const {
  return std::ranges::any_of(boards, [](const Board& b) { return b.winning(); });
}
bool State::losing() const {
  return std::ranges::any_of(boards, [](const Board& b) { return b.losing(); });
}

std::string State::to_string() const {
  // std::string result{'b'};
  std::string result;

  std::string sep;
  for (Board board : boards) {
    result.append(sep);
    result.append(board.to_string());
    sep = " ";
  }
  return result;
}

State State::random() {
  State state;
  for (Board& board : state.boards) {
    board = Board::random();
  }
  return state;
}

State State::from_string(std::string state_string) {
  State state;

  std::stringstream stream(state_string);

  char player;
  stream >> player;

  int i = 0;
  std::string board_string;
  while (stream >> board_string) {
    state.boards[i] = Board::from_string(board_string);
    i++;
  }

  if (player == 'w') state.flip();
  return state;
}

void State::flip() {
  std::swap(boards[0], boards[2]);
  std::swap(boards[1], boards[3]);
  for (Board board : boards) {
    board.flip();
  }
}

std::vector<Move> State::all_moves() const {
  std::vector<Move> moves;
  for (int i : {0, 1}) {
    for (Action passive : boards[i].passive_actions()) {
      for (int j : {i^1, i^3}) {
        for (Action aggressive : boards[j].aggressive_actions(passive.vector)) {
          Move move(i, j, passive, aggressive);
          moves.push_back(move);
        }
      }
    }
  }
  return moves;
}

void State::make(Move move) {
  boards[move.passive_board].make({move.passive_index, move.vector});
  boards[move.aggressive_board].make({move.aggressive_index, move.vector});
  flip();
}

// TODO: consider zobrist hashing
uint64_t State::hash() const {
  return hash::fnv1a_64(to_binary());
}

__uint128_t State::to_binary() const {
  __uint128_t mask = 0;
  for (Board board : boards) {
    mask = (mask << 32) | board.to_binary();
  }
  return mask;
}

float State::evaluate() const {
  float score = 0;
  for (int i = 0; i < 4; ++i) {
    score += boards[i].evaluate();
  }
  return score;
}
