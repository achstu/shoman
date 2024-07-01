#include <iostream>
#include <string>
#include <format>

#include "state.h"
#include "search.h"

int main() {
  std::string state_string;
  while (getline(std::cin, state_string)) {
    State state(state_string);

    std::cout << std::format("eval: {}\tstate: {}",
      state.evaluate(),
      state.to_string()
    ) << std::endl;
  }
}

