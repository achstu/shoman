#include <iostream>
#include <string>
#include <format>

#include "state.h"
#include "search.h"
#include "tt.h"

int main() {
  TranspositionTable* tt = new TranspositionTable();
  if (tt == nullptr) exit(-1);
  
  std::string state_string;
  while (getline(std::cin, state_string)) {
    
    State state = State::from_string(state_string);

    float score = tt_alphabeta(*tt, state, 3);
    
    // std::cout << score << '\n';
    std::cout << tt->operator[](state).move.to_string() << std::endl;
    
    /*
    std::cout << std::format("eval: {:.2f} search: {:.2f} state: {}",
      state.evaluate(),
      search(state, 3),
      state.to_string()
    ) << std::endl;
    */
  }
}

