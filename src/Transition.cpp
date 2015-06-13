#include "Transition.h"

Transition::Transition(int destinationId, char mode, LexemSet *triggers) {
  this->destinationId = destinationId;
  this->mode = mode;
  this->triggers = triggers;
}

bool Transition::equals(Transition *trans) {
  return (destinationId == trans->destinationId) &&
         (mode == trans->mode) &&
         (triggers->equals(trans->triggers));
}

void Transition::prettyPrint() {
  triggers->prettyPrint();
  printf(" -> %c%d", mode, destinationId);
}
