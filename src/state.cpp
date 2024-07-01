#include "state.h"
#include "board.h"

#include <format>
#include <sstream>

#include <algorithm>
#include <ranges>

bool State::terminal() const { return winning() || losing(); }
bool State::winning() const {
  return std::ranges::any_of(boards, [](Board b) { return b.winning(); });
  for (Board board : boards) {
    if (board.winning()) return true;
  }
  return false;
}
bool State::losing() const {
  for (Board board : boards) {
    if (board.losing()) return true;
  }
  return false;
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


State::State(std::string state_string) {
  std::stringstream stream(state_string);

  char player;
  stream >> player;

  int i = 0;
  std::string board_string;
  while (stream >> board_string) {
    boards[i] = Board(board_string);
    i++;
  }

  if (player == 'w') flip();
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
std::size_t State::hash() const {
  const std::size_t M = 998'244'353;
  __uint128_t bb = 0;
  for (Board board : boards) {
    bb = (bb << 32) | board.hash();
  }
  return bb % M;
}

float State::evaluate() const {
  float score = 0;
  for (int i = 0; i < 4; ++i) {
    score += boards[i].evaluate();
  }
  return score;
}
