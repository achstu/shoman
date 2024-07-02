#include <algorithm>
#include <iostream>
#include <string>
#include <format>

#include "state.h"
#include "search.h"

void test_rand_evals(int N) {
  std::vector<float> evals;
  for (int i = 0; i < N; i++) {
    State state(i);
    
    std::cout << std::format("eval: {:.2f}\tstate: {}",
      state.evaluate(),
      state.to_string()
    ) << std::endl;

    evals.push_back(state.evaluate());
  }
  std::ranges::sort(evals);
  std::cout << evals.front() << ' ' << evals.back() << '\n';
}

int main() {

  std::string state_string;
  while (getline(std::cin, state_string)) {
    State state(state_string);

    std::cout << std::format("eval: {:.2f} search: {:.2f} state: {}",
      state.evaluate(),
      search(state, 3),
      state.to_string()
    ) << std::endl;
  }
}

