#include "Transition.h"

Transition::Transition(int destinationId, char mode, Set *transitions) {
  this->destinationId = destinationId;
  this->mode = mode;
  this->transitions = transitions;
}

bool Transition::equals(Transition *trans) {
  return (destinationId == trans->destinationId) &&
         (mode == trans->mode) &&
         (transitions->equals(trans->transitions));
}

void Transition::prettyPrint() {
  transitions->prettyPrint();
  printf(" -> %c%d", mode, destinationId);
}
