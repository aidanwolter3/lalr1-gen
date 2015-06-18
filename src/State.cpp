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

  //check a transition with the same trigger does not exist
  int i;
  for(i = 0; i < numTransitions; i++) {
    if(transitions[i]->trigger->compare(trans->trigger) == 0) {
      break;
    }
  }

  //if does not exist yet, add it
  if(i == numTransitions) {
    transitions[numTransitions++] = trans;
  }

  //if exactly the same, do nothing
  else if(transitions[i]->equals(trans) == true) {
    return;
  }

  //otherwise ask the user whether to reduce or shift
  else {
    return;
    printf("Transition conflict found in State %d\n", id);
    prettyPrint();
    printf("\nConflicting transitions:\n");
    printf("\t1. ");
    transitions[i]->prettyPrint();
    printf("\n\t2. ");
    trans->prettyPrint();
    printf("\n");

    bool validSelection = false;
    while(!validSelection) {
      printf("Please choose a transition to keep (1|2): ");
      char line[256];
      scanf("%s", line);
      if(line[0] == '1' || line[0] == '2') {
        if(line[0] == '2') {
          transitions[i] = trans;
        }
        validSelection = true;
      }
    }

    printf("\n");
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
void State::writeToCSV(FILE *file, LexemSet *triggers) {

  //for every possible transition trigger
  for(int i = 0; i < triggers->size; i++) {

    //check if this state has the trigger
    for(int j = 0; j < numTransitions; j++) {
      if(this->transitions[j]->trigger->compare(triggers->items[i]) == 0) {

        //if the trigger is on a prod
        if(triggers->items[i]->l[0] >= 'A' && triggers->items[i]->l[0] <= 'Z') {
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
    if(i < triggers->size-1) {
      fprintf(file, ",");
    }
  }
}
