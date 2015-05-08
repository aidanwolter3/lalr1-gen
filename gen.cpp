//Aidan Wolter
//LALR1 Table Generator
//Description:
//  This program generates an LALR1 parse table from an file describing
//  the productions. The input file must be named `productions`.

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_PROD_STR_LEN 20
#define MAX_FOLLOW_SET_LEN 10
#define MAX_NUM_PRODS_IN_STATE 100
#define MAX_NUM_PRODS 20
#define MAX_NUM_STATES 100

//A single production which holds the left origin and right derivations. The
//object tracks how far the production has been parsed with a mark.
class Production {
  public:

    // ex. 'S -> .ABC'
    char left;  // ex. 'S'
    char *right;// ex. 'ABC'
    int mark;   // ex. 0
    char *followSet;

    //general constructor
    Production(char left, char *right) {
      this->left = left;
      this->right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
      strcpy(this->right, right);
      this->mark = 0;
      this->followSet = (char*)malloc(MAX_FOLLOW_SET_LEN*sizeof(char));
    }

    //duplicate the production
    Production* duplicate() {
      Production *prod = new Production(this->left, this->right);
      prod->mark = this->mark;
      return prod;
    }

    //print the production
    void prettyPrint() {
      char *markedStr = strdup(this->right);
      strcpy(&markedStr[mark+1], &markedStr[mark]);
      markedStr[mark] = '.';
      printf("%c -> %s\n", this->left, markedStr);
    }

    //check for equality in productions
    bool equals(Production *prod) {
      return (prod->left == this->left) &&
             (prod->mark == this->mark) &&
             (strcmp(prod->right, this->right) == 0) &&
             (strcmp(prod->followSet, this->followSet) == 0);
    }
};

//A parsing state that contains several productions
class State {
  public:
    bool completed;
    int id;
    int numProds;
    Production **prods;

    //general constructor
    State(int id) {
      this->completed = false;
      this->id = id;
      this->numProds = 0;
      this->prods = (Production**)malloc(MAX_NUM_PRODS_IN_STATE*sizeof(Production*));
    }

    //add another production. duplicate to ensure that we do not override anything
    //in other states.
    void addProduction(Production* prod) {

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

    //print the state nicely
    void prettyPrint() {
      printf("\n");
      printf("State: %d\n", this->id);
      printf("-------------------\n");
      for(int i = 0; i < this->numProds; i++) {
        this->prods[i]->prettyPrint();
      }
      printf("\n");
    }
};

//program entrance
int main(int argc, char* argv[]) {

  //hold all the productions from the file
  Production *generalProductions[MAX_NUM_PRODS];
  int productionCount = 0;

  //keep track of all states
  State *states[MAX_NUM_STATES];
  int stateCount = 0;

  //read the production file
  FILE *prod_file = fopen("productions", "r");
  char left;
  char *right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
  while(fscanf(prod_file, "%c -> %s\n", &left, right) > 0) {
    generalProductions[productionCount++] = new Production(left, right);
  }

  //print the read productions
  //for(int i = 0; i < productionCount; i++) {
  //  generalProductions[i]->prettyPrint();
  //}

  //create an initial state with the first prod
  states[stateCount] = new State(stateCount);
  stateCount++;
  states[0]->addProduction(generalProductions[0]);

  //start with the first state
  int currentState = 0;

  //if state was found not completed
  if(currentState != stateCount) {

    //recursively add the prods to the state with the follow sets
    //check each production in the state (and each newly added)
    for(int i = 0; i < states[currentState]->numProds; i++) {
      
      //get the next symbol on the production
      int idx = states[currentState]->prods[i]->mark;
      char n = states[currentState]->prods[i]->right[idx];

      //compare each production's next symbol with the start of a
      //general production
      for(int j = 0; j < productionCount; j++) {
        if(generalProductions[j]->left == n) {
          states[currentState]->addProduction(generalProductions[j]);
        }
      }
    }

    //while state has prod
      //read next symbol
      //create state with ID
      //add connection to state in current state with ID and symbol
      //find all prods with symbol and move to new state (rm and add)
      //add state to list

    //mark current state as completed
    states[currentState]->completed = true;

    //find next state not completed
    for(currentState = 0; currentState < stateCount; currentState++) {
      if(states[currentState]->completed == false) {
        break;
      }
    }
  }

  //print all the states
  for(int i = 0; i < stateCount; i++) {
    states[i]->prettyPrint();
  }

  return 0;
}
