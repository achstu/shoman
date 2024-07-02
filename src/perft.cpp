#include <iostream>
#include <algorithm>

#include "state.h"
#include "tt.h"

#include <format>

void evaluation_test() {
  std::cout << "Testing evaluation function..." << std::endl;
  
  std::vector<float> evals;
  for (int i = 0; i < 1000; i++) {
    State state = State::random();
    
    if (state.terminal()) continue;
    
    float score = state.evaluate();
    std::cout << std::format("eval: {:.2f}\tstate: {}",
      score,
      state.to_string()
    ) << std::endl;

    evals.push_back(score);
  }
  std::ranges::sort(evals);
  std::cout << evals.front() << ' ' << evals.back() << std::endl;
}

void collision_test() {
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
  // evaluation_test();
  collision_test();
}

