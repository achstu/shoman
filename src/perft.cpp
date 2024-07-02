#include <iostream>

#include "board.h"
#include "state.h"
#include "tt.h"

#include <format>

void collision_test() {
  std::ios_base::sync_with_stdio(false);
  
  std::cout << "Startning collision test..." << std::endl;

  TT* trans = new TT();

  int collisions = 0;
  int n = 100'000;
  for (int i = 0; i < n; i++) {
    State state = State::random();
    if (trans->contains(state)) {
      State key = trans->get_key(state);
      
      if (key != state) {
        std::cout << "#### hash collision ####\n";
        std::cout << std::format("{} {:08X} {}\n{} {:08X} {}\n",
                                 key.to_string(), key.hash(), key.hash() % TT::SIZE,
                                 state.to_string(), state.hash(), state.hash() % TT::SIZE
                               );
        std::cout << "########################\n\n";
        collisions += 1;
      }
    }
    trans->insert(state, {+1.0, 1});
  }

  std::cout << std::format("Total collisions: {}/{}", collisions, n) << std::endl;

  delete trans;
}

int main() {
  collision_test();
}

