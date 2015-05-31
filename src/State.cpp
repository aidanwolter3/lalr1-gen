#include "State.h"

//general constructor
State::State(int id) {
  this->completed = false;
  this->id = id;
  this->numProds = 0;
  this->prods = (Production**)malloc(MAX_NUM_PRODS_IN_STATE*sizeof(Production*));
  this->numTransitions = 0;
}

//add another production. duplicate to ensure that we do not override anything
//in other states.
void State::addProduction(Production *prod) {

  //ensure the production does not already exist in the state
  int i;
  for(i = 0; i < numProds; i++) {
    if(prods[i]->equals(prod) == true) {
      break;
    }
  }
  
  //does not exist yet, so add it
  if(i == numProds) {
    prods[numProds++] = prod->duplicate();
  }
}

void State::addTransition(Transition *trans) {

  //ensure the transition does not already exist in the state
  int i;
  for(i = 0; i < numTransitions; i++) {
    if(transitions[i]->equals(trans) == true) {
      break;
    }
  }

  //does not exist yet, so add it
  if(i == numTransitions) {
    transitions[numTransitions++] = trans;
  }
}

//print the state nicely
void State::prettyPrint() {
  printf("\n");
  printf("State: %d\n", this->id);
  printf("-------------------\n");
  for(int i = 0; i < numTransitions; i++) {
    transitions[i]->prettyPrint();
    if(i < numTransitions-1) {
      printf(", ");
    }
  }
  printf("\n");
  printf("-------------------\n");
  for(int i = 0; i < numProds; i++) {
    prods[i]->prettyPrint();
  }
  printf("\n");
}

//write the states line in a csv for the compiler to read
void State::writeToCSV(FILE *file, Set *transitions) {
  for(int i = 0; i < transitions->size; i++) {
    for(int j = 0; j < numTransitions; j++) {
      if(this->transitions[j]->transitions->has(transitions->items[i])) {
        if(transitions->items[i] >= 'A' && transitions->items[i] <= 'Z') {
          fprintf(file, "%d", this->transitions[j]->destinationId);
        }
        else {
          fprintf(file, "%c%d", this->transitions[j]->mode, this->transitions[j]->destinationId);
        }
        break;
      }
    }
    if(i < transitions->size-1) {
      fprintf(file, ",");
    }
  }
}
