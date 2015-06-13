//Aidan Wolter
//LALR1 Table Generator
//Description:
//  This program generates an LALR1 parse table from an file describing
//  the language. The input file must be named `language_definition`
//  and be formatted like below with lexems listed first then
//  productions.
//
//  %%
//
//  S -> 1
//  A -> 2
//  B -> 3
//  E -> 4
//
//  %%
//
//  S -> A
//  A -> B
//  A -> BAB
//  B -> E
//
//  %%

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "src/LanguageDefinitionParser.h"
#include "src/Production.h"
#include "src/Transition.h"
#include "src/State.h"
#include "src/FirstSets.h"

#define MAX_NUM_STATES 100
#define MAX_NUM_TRANSITIONS 1000

//program entrance
int main(int argc, char* argv[]) {

  LanguageDefinitionParser *parser = new LanguageDefinitionParser();

  //create a place for all the lexems
  Lexem *lexems[MAX_NUM_LEXEMS];
  int lexemCount = 0;

  //create a place to store all the productions
  Production *generalProductions[MAX_NUM_PRODS];
  int productionCount = 0;

  parser->parse("language_definition", generalProductions, &productionCount);

  //keep track of all states
  State *states[MAX_NUM_STATES];
  int stateCount = 0;

  //keep track of all possible transitions
  LexemSet *allTransitions = new LexemSet();

  //generate the first sets for use later when determining follow sets
  FirstSets *firstSets = new FirstSets(generalProductions, productionCount);
  firstSets->generate();

  //create an initial state with the first prod
  states[stateCount] = new State(stateCount);
  stateCount++;
  states[0]->addProduction(generalProductions[0]);
  Lexem *eofLexem = new Lexem(-1, (char*)"eof");
  states[0]->prods[0]->followSet->add(eofLexem);

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
      if(idx >= curProd->right->size) {
        continue;
      }

      //get the next symbol on the production
      Lexem *n = curProd->right->items[idx];

      //compare each production's next symbol with the start of a
      //general production
      for(int j = 0; j < productionCount; j++) {

        if(generalProductions[j]->left->compare(n) == 0) {
          Production *newProd = generalProductions[j]->duplicate();

          //create the follow set
          //while(next is not char and not end of prod), get the first set at mark
          int mark = idx+1;
          bool finishedFollow = false;
          while(finishedFollow == false && mark != curProd->right->size && (curProd->right->items[mark]->l[0] >= 'A' && curProd->right->items[mark]->l[0] <= 'Z')) {
            
            //add the first of the production to the new follow set
            Lexem *l = curProd->right->items[mark];
            LexemSet *first = firstSets->firstForProd(l)->duplicate();
            Lexem *epsilon = new Lexem(0, (char*)"epsilon");
            finishedFollow = first->remove(epsilon) == 0;
            newProd->followSet->add(first);

            //move to next symbol and get the first set
            mark++;
          }

          //if next is end of prod
          if(finishedFollow == false) {
            if(mark == curProd->right->size) {

              //add follow of current prod to new prod
              newProd->followSet->add(curProd->followSet);
            }

            //if next is char
            else {

              //add char to follow set
              newProd->followSet->add(curProd->right->items[mark]);
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
      if(idx < curProd->right->size) {

        //get the next symbol on the production
        Lexem *n = curProd->right->items[idx];

        //add the symbol as a possible transition
        if(allTransitions->has(n) == -1) {
          allTransitions->add(n);
        }

        //create state with ID and production
        State *newState = new State(stateCount);
        newState->addProduction(curProd);

        //find all prods with same symbol and move to the new state
        for(int i = currentProdIndex+1; i < curState->numProds; i++) {
          int markIndex = curState->prods[i]->mark;

          //if the production has the same next symbol
          if(curState->prods[i]->right->items[markIndex]->compare(n) == 0) {
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
        LexemSet *transitionSet = new LexemSet();
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

  if(allTransitions->has(eofLexem) == -1) {
    allTransitions->add(eofLexem);
  }

  //print all the states to the console
  for(int i = 0; i < stateCount; i++) {
    states[i]->prettyPrint();
  }

  //write the parse table csv
  FILE *csvFile = fopen("parse_table.csv", "w");

  //write the transition strings
  for(int i = 0; i < allTransitions->size; i++) {
    fprintf(csvFile, "%s", allTransitions->items[i]->l);
    if(i < allTransitions->size-1) {
      fprintf(csvFile, ",");
    }
  }
  fprintf(csvFile, "\r");

  //write the lex ids
  for(int i = 0; i < allTransitions->size; i++) {
    fprintf(csvFile, "%d", allTransitions->items[i]->t);
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

  fclose(csvFile);

  return 0;
}
