#ifndef STATE
#define STATE

#include "Production.h"
#include "Transition.h"

#define MAX_NUM_PRODS_IN_STATE 100

//A parsing state that contains several productions
class State {
  public:
    bool completed;
    int id;
    int numProds;
    Production **prods;
    Transition *transitions[MAX_NUM_PRODS_IN_STATE];
    int numTransitions;

    //general constructor
    State(int id);

    //add another production. duplicate to ensure that we do not override anything
    //in other states.
    void addProduction(Production *prod);

    void addTransition(Transition *trans);

    //print the state nicely
    void prettyPrint();

    //write the states line in a csv for the compiler to read
    void writeToCSV(FILE *file, LexemSet *transitions);
};

#endif
