#include "move.h"


#include <format>


Move::Move(int i, int j, Action passive, Action aggressive) {
  // assert(passive.vector == aggressive.vector);
  
  vector = passive.vector; // == aggressive.vector;
  passive_board = i;
  passive_index = passive.index;

  aggressive_board = j;
  aggressive_index = aggressive.index;
}
  
std::string Move::to_string() const {
  return std::format("{}{}{}{}{}",
    vector.to_string(),
    passive_board == 1 ? 'w' : 'b',
    passive_index,
    aggressive_board >> 1 ? 'f' : 'h',
    aggressive_index
  );
}
