#pragma once

#include <string>

struct Move {
  int length;
  int vector;
  int passive_board;
  int passive_index;
  int aggressive_board;
  int aggressive_index;

  
  std::string to_string() const;
/*
  int lenght() const;
  int vector() const;

  int passive_board() const;
  int passive_index() const;

  int aggressive_board() const;
  int aggressive_index() const;
*/
};
