#include <iostream>

#include "board.h"
#include "state.h"
#include "tt.h"



void collision_test() {
  std::cout << "Startning collision test..." << std::endl;

  TT trans;
  return;

  int collisions = 0;
  int n = 1000;
  for (int i = 0; i < n; i++) {
    State state = State::random();
    if (trans.contains(state)) {
      State key = trans.get_key(state);
      
      if (key != state) {
        std::cout << "Hash collision occured" << std::endl;
        collisions += 1;
      }
    }
    trans.insert(state, {+1.0, 1});
    std::cout << state.to_string() << ' ' << state.hash() << std::endl;
  }

  std::cout << "Total collisions: " << collisions << std::endl;
}

int main() {

  std::cout << sizeof(State) << std::endl;
  std::cout << sizeof(Board) << std::endl;
  std::cout << sizeof(TT::Elem) << std::endl;
  
  collision_test();
}

