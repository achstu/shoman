#include "state.h"

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

// std::string State::to_string() const {
//   std::string result = std::format("{}: ", 'b');
//   for (Board board : boards) {
//     result.push_back(' ');
//     result.append(board.to_string());
//   }
//   return result;
// }


State::State(std::string state_string) {
  std::stringstream stream(state_string);

  char player;
  stream >> player;
  if (player == 'w') std::swap(boards[0], boards[1]);

  int i = 0;
  std::string board_string;
  while (stream >> board_string) {
    boards[i] = Board(board_string);
    i++;
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
