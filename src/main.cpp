#include <algorithm>
#include <iostream>
#include <string>
#include <format>

#include "state.h"
#include "search.h"

int main() {

  std::vector<float> evals;
  for (int i = 0; i < 100000; i++) {
    State state(i);
    
    std::cout << std::format("eval: {:.2f}\tstate: {}",
      state.evaluate(),
      state.to_string()
    ) << std::endl;

    evals.push_back(state.evaluate());
  }
  std::ranges::sort(evals);
  std::cout << evals.front() << ' ' << evals.back() << '\n';

  return 0;
  std::string state_string;
  while (getline(std::cin, state_string)) {
    State state(state_string);

    std::cout << std::format("eval: {}\t\tstate: {}",
      state.evaluate(),
      state.to_string()
    ) << std::endl;
  }
}

