#pragma once

#include "vector.h"

struct Action {
  int index;
  Vector vector;

  int start() const;
  int mid() const;
  int end() const;
  int after() const;
};
