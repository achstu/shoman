#pragma once

#include <string>


struct Vector {
  int length;
  int shift;

  std::string to_string() const;
};

const Vector vectors[] = {
  // L and R
  {1, -1}, {2, -1},
  {1, +1}, {2, +1},
  // U and D
  {1, -4}, {2, -4},
  {1, +4}, {2, +4},
  // UR and DL
  {1, -3}, {2, -3},
  {1, +3}, {2, +3},
  // UL and DR
  {1, -5}, {2, -5},
  {1, +5}, {2, +5}
};


struct Action {
  int index;
  Vector vector;

  std::string to_string() const;

  int mid() { return index + vector.shift; }
  int end() { return index + vector.length * vector.shift; }
  int after() { return end() + vector.shift; }
};
