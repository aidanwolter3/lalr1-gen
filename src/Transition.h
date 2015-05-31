#ifndef TRANSITION
#define TRANSITION

#include "Set.h"

//represents a transition from one state to another
class Transition {
  public:
    int destinationId;
    char mode; //s = shift, r = reduce
    Set *transitions;

    Transition(int destinationId, char mode, Set *transitions);

    bool equals(Transition *trans);

    void prettyPrint();
};

#endif
