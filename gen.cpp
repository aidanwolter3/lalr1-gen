//Aidan Wolter
//LALR1 Table Generator
//Description:
//  This program generates an LALR1 parse table from an file describing
//  the productions. The input file must be named `productions` and be
//  formatted like below.
//
//  S -> A
//  A -> B
//  A -> BAB
//  B -> E

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "lexer.h"

#define MAX_PROD_STR_LEN 20
#define MAX_FOLLOW_SET_LEN 10
#define MAX_NUM_PRODS_IN_STATE 100
#define MAX_NUM_PRODS 20
#define MAX_NUM_STATES 100

//A follow set for a production
class Set {
  public:
    char *items;
    int size;

    //general constructor
    Set() {
      items = (char*)malloc(MAX_FOLLOW_SET_LEN*sizeof(char));
      size = 0;
    }

    //sort the set for easy comparison
    void sort() {
      for(int i = 0; i < size-1; i++) {
        int min = i;

        //check for smaller
        for(int j = i+1; j < size; j++) {
          if(items[j] < items[min]) {
            min = j;
          }
        }
        
        //swap
        int tmp = items[i];
        items[i] = items[min];
        items[min] = tmp;
      }
    }

    //add a new follow to the set. do not allow duplicates. sort the set afterwards
    void add(char c) {

      //check for duplicates
      int i;
      for(i = 0; i < size; i++) {
        if(items[i] == c) {
          break;
        }
      }

      //check all items, so no duplicates
      if(i == size) {
        items[size] = c;
        size++;
        sort();
      }
    }

    //add all the items in one set to this one
    void add(Set *s) {
      for(int i = 0; i < s->size; i++) {
        add(s->items[i]);
      }
    }

    //remove a specific item
    int remove(char c) {
      for(int i = 0; i < size; i++) {
        if(items[i] == c) {
          strcpy(&items[i], &items[i+1]);
          size--;
          return 1;
        }
      }
      return 0;
    }

    //check for equality between sets
    bool equals(Set *f) {
      return (strcmp(f->items, items) == 0);
    }

    //print the follow set
    void prettyPrint() {
      printf("{%s}", items);
    }

    //set setter. copies string instead of reusing
    void set(char *items) {
      strcpy(this->items, items);
      size = strlen(items);
    }

    //duplicate the follow set
    Set* duplicate() {
      Set *newSet = new Set();
      newSet->set(items);
      return newSet;
    }
};

//A single production which holds the left origin and right derivations. The
//object tracks how far the production has been parsed with a mark.
class Production {
  public:

    // ex. 'S -> .ABC'
    char left;  // ex. 'S'
    char *right;// ex. 'ABC'
    int mark;   // ex. 0
    Set *followSet;

    //track whether this production has already been check so we do not have
    //to bother removing it from the state
    bool completed;

    //general constructor
    Production(char left, char *right) {
      this->left = left;
      this->right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
      strcpy(this->right, right);
      this->mark = 0;
      this->followSet = new Set();
      this->completed = false;
    }

    //duplicate the production
    Production* duplicate() {
      Production *prod = new Production(left, right);
      prod->mark = mark;
      prod->followSet = followSet->duplicate();
      return prod;
    }

    //print the production
    void prettyPrint() {
      char *markedStr = strdup(this->right);
      strcpy(&markedStr[mark+1], &markedStr[mark]);
      markedStr[mark] = '.';
      printf("%c -> %s  ", left, markedStr);
      followSet->prettyPrint();
      printf("\n");
    }

    //check for equality in productions
    bool equals(Production *prod) {
      return (prod->left == this->left) &&
             (prod->mark == this->mark) &&
             (strcmp(prod->right, this->right) == 0) &&
             (prod->followSet->equals(this->followSet) == true);
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

//a class which computes all the first sets recursively for the productions
class FirstSets {
  private:
    Production **prods;
    int productionCount;

  public:
    char lefts[MAX_NUM_PRODS];
    Set *sets[MAX_NUM_PRODS];
    int size;

    //general constructor
    FirstSets(Production *prods[], int productionCount) {
      size = 0;
      this->prods = prods;
      this->productionCount = productionCount;
    }

    //generate all first sets
    void generate() {
      for(int i = 0; i < productionCount; i++) {
        firstForProd(prods[i]->left);
      }
    }

    //recursive function that generates a single first set by generating others
    Set* firstForProd(char c) {

      //check is the first set already exists and return
      for(int i = 0; i < size; i++) {
        if(lefts[i] == c) {
          return sets[i];
        }
      }

      //create a new first set
      Set *firstSet = new Set();

      //check every production
      for(int i = 0; i < productionCount; i++) {

        //if the production has the left symbol c
        if(prods[i]->left == c) {

          //if the first symbol is a production (capital letter)
          char firstRight = prods[i]->right[0];
          if(firstRight >= 'A' && firstRight <= 'Z') {

            //continue if the next production is the same as the original
            if(firstRight == c) {
              continue;
            }

            //otherwise get the set of the production
            firstSet->add(firstForProd(firstRight));
          }

          //if the first symbol is a char add it to the set
          else {
            firstSet->add(firstRight);
          }
        }
      }

      //add the set to the list and return
      lefts[size] = c;
      sets[size] = firstSet;
      size++;
      return firstSet;
    }

    //print the first sets nicely
    void prettyPrint() {
      printf("First sets:\n");
      for(int i = 0; i < size; i++) {
        printf("%c ", lefts[i]);
        sets[i]->prettyPrint();
        printf("\n");
      }
    }
};

//program entrance
int main(int argc, char* argv[]) {

  Lexer *lexer = new Lexer((char*)"productions");
  lexer->nextToken()->prettyPrint();
  lexer->nextToken()->prettyPrint();
  lexer->nextToken()->prettyPrint();
  lexer->nextToken()->prettyPrint();
  lexer->nextToken()->prettyPrint();
  lexer->nextToken()->prettyPrint();
  return 0;

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

  //generate the first sets for use later when determining follow sets
  FirstSets *firstSets = new FirstSets(generalProductions, productionCount);
  firstSets->generate();
  firstSets->prettyPrint();

  //create an initial state with the first prod
  states[stateCount] = new State(stateCount);
  stateCount++;
  states[0]->addProduction(generalProductions[0]);
  states[0]->prods[0]->followSet->add('F');

  //start with the first state
  int currentStateIndex = 0;

  //if state was found incomplete
  while(currentStateIndex < stateCount) {

    //get current state
    State *curState = states[currentStateIndex];

    //recursively add the prods to the state with the follow sets
    //check each production in the state (and each newly added)
    for(int i = 0; i < curState->numProds; i++) {

      //get current production
      Production *curProd = curState->prods[i];
      
      //get index of the next symbol on the production
      int idx = curProd->mark;

      //do not check production if marker is at the end
      if(idx >= strlen(curProd->right)) {
        continue;
      }

      //get the next symbol on the production
      char n = curProd->right[idx];

      //compare each production's next symbol with the start of a
      //general production
      for(int j = 0; j < productionCount; j++) {
        if(generalProductions[j]->left == n) {
          Production *newProd = generalProductions[j]->duplicate();

          //create the follow set
          //while(next is not char and not end of prod), get the first set at mark
          int mark = idx+1;
          bool finishedFollow = false;
          while(finishedFollow == false && mark != strlen(curProd->right) && (curProd->right[mark] >= 'A' && curProd->right[mark] <= 'Z')) {
            
            //add the first of the production to the new follow set
            char l = curProd->right[mark];
            Set *first = firstSets->firstForProd(l);
            finishedFollow = first->remove('.') == 1;
            newProd->followSet->add(first);

            //move to next symbol and get the first set
            mark++;
          }

          //if next is end of prod
          if(finishedFollow == false) {
            if(mark == strlen(curProd->right)) {

              //add follow of current prod to new prod
              newProd->followSet->add(curProd->followSet);
            }

            //if next is char
            else {

              //add char to follow set
              newProd->followSet->add(curProd->right[mark]);
            }
          }

          curState->addProduction(newProd);
        }
      }
    }

    //branch off and create new states
    //start with the first production
    int currentProdIndex = 0;

    //if state was found incomplete
    while(currentProdIndex < curState->numProds) {

      //get current production
      Production *curProd = curState->prods[currentProdIndex];
      curProd->completed = true;

      //get index of the next symbol on the production
      int idx = curProd->mark;

      //do not check production if marker is at the end
      if(idx < strlen(curProd->right)) {

        //get the next symbol on the production
        char n = curProd->right[idx];

        //create state with ID and production
        State *newState = new State(stateCount);
        newState->addProduction(curProd);

        //find all prods with same symbol and move to the new state
        for(int i = currentProdIndex+1; i < curState->numProds; i++) {
          int markIndex = curState->prods[i]->mark;

          //if the production has the same next symbol
          if(curState->prods[i]->right[markIndex] == n) {
            newState->addProduction(curState->prods[i]);
            curState->prods[i]->completed = true;
          }
        }

        //advance all marks up one for prods in state
        for(int i = 0; i < newState->numProds; i++) {
          newState->prods[i]->mark++;
        }

        //ensure that the state does not already exist
        //only check the productions in the newState
        bool match = true;
        for(int i = 0; i < stateCount; i++) {
          match = true;

          //if the state to compare has less productions, then don't even check
          if(newState->numProds > states[i]->numProds) {
            match = false;
            continue;
          }

          for(int p = 0; p < newState->numProds; p++) {
            if(newState->prods[p]->equals(states[i]->prods[p]) == false) {
              match = false;
              break;
            }
          }
          if(match == true) {
            break;
          }
        }

        //add the state to the list because it is not a duplicate
        if(match == false) {
          states[stateCount] = newState;
          stateCount++;
        }

        //TODO: add connection to state in current state with ID and symbol

      }

      //TODO: reduce
      else {
      }

      //find next incomplete production
      for(currentProdIndex = 0; currentProdIndex < curState->numProds; currentProdIndex++) {
        if(curState->prods[currentProdIndex]->completed == false) {
          break;
        }
      }
    }

    //mark current state as completed
    curState->completed = true;

    //find next incompleted state
    for(currentStateIndex = 0; currentStateIndex < stateCount; currentStateIndex++) {
      if(states[currentStateIndex]->completed == false) {
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
