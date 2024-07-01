#include <iostream>
#include <string>

#include "state.h"
#include "move.h"
#include "search.h"

int main() {
  std::string state_string;
  getline(std::cin, state_string);
  State state(state_string);

  std::cout << minimax(state) << std::endl;
}

