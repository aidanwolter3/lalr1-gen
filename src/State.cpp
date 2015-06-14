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
    printf("\n");
  }
}

//write the states line in a csv for the compiler to read
void State::writeToCSV(FILE *file, LexemSet *transitions) {

  //for every possible transition trigger
  for(int i = 0; i < transitions->size; i++) {

    //check if this state has the trigger
    for(int j = 0; j < numTransitions; j++) {
      if(this->transitions[j]->triggers->has(transitions->items[i]) >= 0) {

        //if the trigger is on a prod
        if(transitions->items[i]->l[0] >= 'A' && transitions->items[i]->l[0] <= 'Z') {
          fprintf(file, "%d", this->transitions[j]->destinationId);
        }

        //else the trigger is on a token
        else {
          if(this->transitions[j]->mode == 'r' && this->transitions[j]->destinationId == 0) {
            fprintf(file, "a");
          }
          else {
            fprintf(file, "%c%d", this->transitions[j]->mode, this->transitions[j]->destinationId);
          }
        }
        break;
      }
    }
    if(i < transitions->size-1) {
      fprintf(file, ",");
    }
  }
}
