#ifndef TRANSITION
#define TRANSITION

#include "LexemSet.h"

//represents a transition from one state to another
class Transition {
  public:
    int destinationId;
    char mode; //s = shift, r = reduce
    LexemSet *triggers;

    Transition(int destinationId, char mode, LexemSet *triggers);

    bool equals(Transition *trans);

    void prettyPrint();
};

#endif
