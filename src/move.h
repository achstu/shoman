#pragma once

#include <string>

struct Move {
  std::string notation;

  Move(std::string );
  std::string to_string();

  int lenght() const;
  int vector(const int side) const;

  int passive(char player) const;
  int passive_index() const;
  
  int aggressive(char player) const;
  int aggressive_index() const;
};
