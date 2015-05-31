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

#include "src/Set.h"
#include "src/Production.h"
#include "src/Transition.h"
#include "src/State.h"
#include "src/FirstSets.h"

#define MAX_NUM_STATES 100

//program entrance
int main(int argc, char* argv[]) {

  //create a place to store all the productions
  Production *generalProductions[MAX_NUM_PRODS];
  int productionCount = 0;

  //keep track of all states
  State *states[MAX_NUM_STATES];
  int stateCount = 0;

  //keep track of all possible transitions
  Set *allTransitions = new Set();

  //read the production file
  Lexer *lexer = new Lexer((char*)"productions");
  Token *leftToken = lexer->nextToken();
  while(leftToken->id == 1) {
    Token *assignToken = lexer->nextToken();
    if(assignToken->id == 2) {
      Token *rightToken = lexer->nextToken();
      if(rightToken->id == 1) {
        Token *newlineToken = lexer->nextToken();
        if(newlineToken->id == 0) {
          generalProductions[productionCount] = new Production(productionCount, leftToken->lexem[0], rightToken->lexem);
          productionCount++;
        }
        else {
          printf("Error in production file!\n");
          return 0;
        }
      }
      else if(rightToken->id == 0) {
        generalProductions[productionCount] = new Production(productionCount, leftToken->lexem[0], rightToken->lexem);
          productionCount++;
      }
      else {
        printf("Error in production file!\n");
        return 0;
      }
    }
    else {
      printf("Error in production file!\n");
      return 0;
    }
    //Production *newProd = new Production(leftToken->left);
    leftToken = lexer->nextToken();
  }

  //generate the first sets for use later when determining follow sets
  FirstSets *firstSets = new FirstSets(generalProductions, productionCount);
  firstSets->generate();

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
            Set *first = firstSets->firstForProd(l)->duplicate();
            finishedFollow = first->remove('E') == 0;
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

        //add the symbol as a possible transition
        allTransitions->add(n);

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
        Set *transitionSet = new Set();
        transitionSet->add(n);
        curState->addTransition(new Transition(newState->id, 's', transitionSet));

      }

      //add a reduce transition because end of production
      else {
        curState->addTransition(new Transition(curProd->id, 'r', curProd->followSet));
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

  allTransitions->add('F');

  //print all the states to the console
  for(int i = 0; i < stateCount; i++) {
    states[i]->prettyPrint();
  }

  //write the parse table csv
  FILE *csvFile = fopen("parse_table.csv", "w");

  //write the transition strings
  for(int i = 0; i < allTransitions->size; i++) {
    fprintf(csvFile, "%c", allTransitions->items[i]);
    if(i < allTransitions->size-1) {
      fprintf(csvFile, ",");
    }
  }
  fprintf(csvFile, "\r");

  //write the lex ids
  int lexCount = 1;
  for(int i = 0; i < allTransitions->size; i++) {
    if(allTransitions->items[i] == 'F') {
      fprintf(csvFile, "-3");
    }
    else {
      fprintf(csvFile, "%d", lexCount++);
    }
    if(i < allTransitions->size-1) {
      fprintf(csvFile, ",");
    }
  }
  fprintf(csvFile, "\r");

  //write the states
  for(int i = 0; i < stateCount; i++) {
    states[i]->writeToCSV(csvFile, allTransitions);
    if(i < stateCount-1) {
      fprintf(csvFile, "\r");
    }
  }

  return 0;
}
