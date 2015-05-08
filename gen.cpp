//Aidan Wolter
//LALR1 Table Generator
//Description:
//  This program generates an LALR1 parse table from an file describing
//  the productions. The input file must be named `productions`.

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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
      this->right = (char*)malloc(20*sizeof(char));
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

  Production *prod1 = new Production('S', "ABC");
  Production *prod2 = new Production('A', "DEF");

  State *state = new State(1);
  state->addProduction(prod1);
  state->addProduction(prod2);
  state->prettyPrint();
  
  //read the production file

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
