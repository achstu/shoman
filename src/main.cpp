#include <iostream>
#include <string>

#include "state.h"
#include "search.h"

int main() {
  std::string state_string;
  getline(std::cin, state_string);
  State state(state_string);

  std::cout << minimax(state, 2) << std::endl;
}

