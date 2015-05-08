//Aidan Wolter
//LALR1 Table Generator
//Description:
//  This program generates an LALR1 parse table from an file describing
//  the productions. The input file must be named `productions`.

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_PROD_STR_LEN 20

//A single production which holds the left origin and right derivations. The
//object tracks how far the production has been parsed with a mark.
class Production {
  private:

    // ex. 'S -> .ABC'
    char left;  // ex. 'S'
    char *right;// ex. 'ABC'

  public:
    int mark;   // ex. 0

    //general constructor
    Production(char left, char *right) {
      this->left = left;
      this->right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
      strcpy(this->right, right);
      this->mark = 0;
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
};

//A parsing state that contains several productions
class State {
  public:
    int id;
    int numProds;
    Production **prods;

    //general constructor
    State(int id) {
      this->id = id;
      this->numProds = 0;
      this->prods = (Production**)malloc(100*sizeof(Production*));
    }

    //add another production. duplicate to ensure that we do not override anything
    //in other states.
    void addProduction(Production* prod) {
      prods[numProds++] = prod->duplicate();
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
  Production *generalProductions[20];
  int productionCount = 0;
  
  //read the production file
  FILE *prod_file = fopen("productions", "r");
  char left;
  char *right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
  while(fscanf(prod_file, "%c -> %s\n", &left, right) > 0) {
    generalProductions[productionCount++] = new Production(left, right);
  }

  //print the read productions
  for(int i = 0; i < productionCount; i++) {
    generalProductions[i]->prettyPrint();
  }

  //create an initial state with the first prod
  //add state to list

  //find first state not completed
  //if state not null

    //recursively add the prods to the state with the follow sets

    //while state has prod
      //read next symbol
      //create state with ID
      //add connection to state in current state with ID and symbol
      //find all prods with symbol and move to new state (rm and add)
      //add state to list

    //mark current state as completed

  return 0;
}
