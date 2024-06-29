#include "action.h"

#include <format>

std::string Vector::to_string() const {
  // TODO: trans is mapping from int -> string
  return std::format("{%d}{%s}", length, trans.at(shift));
}
