#include "Transition.h"

Transition::Transition(int destinationId, char mode, Lexem *trigger) {
  this->destinationId = destinationId;
  this->mode = mode;
  this->trigger = trigger;
}

bool Transition::equals(Transition *trans) {
  return (destinationId == trans->destinationId) &&
         (mode == trans->mode) &&
         (trigger->compare(trans->trigger) == 0);
}

void Transition::prettyPrint() {
  printf("{");
  trigger->prettyPrint();
  printf(" -> %c%d", mode, destinationId);
  printf("}");
}
