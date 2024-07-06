#include "action.h"


int Action::start() const { return index; }
int Action::mid() const { return index + vector.shift; }
int Action::end() const { return index + vector.shift * vector.length; }
int Action::after() const { return end() + vector.shift; }

