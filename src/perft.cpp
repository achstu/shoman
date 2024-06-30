#include <iostream>
#include <vector>
#include <set>

#include "state.h"


std::vector<State> leafs;

int leaf_nodes(State state, int depth) {
  if (depth == 0) {
    leafs.push_back(state);
    return 1;
  }

  for (Move move : state.all_moves()) {
    State child = state;
    child.make(move);
    leaf_nodes(child, depth - 1);
  }
  return 1;
}

int main() {
  State initial("b wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb");


  for (int d = 1; d <= 4; d++) {
    leafs.clear();
    leaf_nodes(initial, d);
    
    std::cout << d << ": ";
    std::cout << leafs.size() << ' ';

    std::set<std::size_t> s;
    for (State leaf : leafs) {
      s.insert(leaf.hash());
    }
    std::cout << s.size() << '\n';
  }
}

