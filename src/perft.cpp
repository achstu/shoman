#include <iostream>
#include <format>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include "state.h"



int leaf_nodes(State state, int depth) {
  if (depth == 0) return 1;

  std::vector<State> leafs;  
  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);
    leafs.push_back(child);

    std::cerr << std::format("move: {} {} hash: {}",
      move.to_string(),
      child.to_string(),
      child.hash()
    ) << std::endl;
  }

  std::ranges::sort(leafs, [](State a, State b) {
    return a.hash() < b.hash();
  });
  std::set<std::size_t> s;
  for (State leaf : leafs) {
    // std::cerr << leaf.to_string() << ' ' << leaf.hash() << '\n';
    s.insert(leaf.hash());
  }

  std::cerr << s.size() << '\n';
  return leafs.size();
}

int main() {
  State initial("b wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb");


  std::cout << leaf_nodes(initial, 1);
}

