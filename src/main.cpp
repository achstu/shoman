#include <iostream>
#include <string>

#include "state.h"
#include "search.h"
#include "tt.h"

int main() {
  TranspositionTable* tt = new TranspositionTable();
  if (tt == nullptr) exit(-1);

  std::string state_string;
  while (getline(std::cin, state_string)) {
    
    State state = State::from_string(state_string);
    if (state.terminal()) continue;

    float score = tt_alphabeta(*tt, state, 3);
    
    std::cout << tt->operator[](state).move.to_string() << std::endl;
  }
}

