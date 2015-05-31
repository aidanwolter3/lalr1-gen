#include "FirstSets.h"

//general constructor
FirstSets::FirstSets(Production *prods[], int productionCount) {
  size = 0;
  this->prods = prods;
  this->productionCount = productionCount;
}

//generate all first sets
void FirstSets::generate() {
  for(int i = 0; i < productionCount; i++) {
    firstForProd(prods[i]->left);
  }
}

//recursive function that generates a single first set by generating others
Set* FirstSets::firstForProd(char c) {

  //check if the first set already exists and return
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
      char firstRight = prods[i]->right[0];

      //if the first symbol is a production (capital letter)
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

        //if epsilon, then modify the char
        if(strlen(prods[i]->right) == 0) {
          firstRight = 'E';
        }

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
void FirstSets::prettyPrint() {
  printf("First sets:\n");
  for(int i = 0; i < size; i++) {
    printf("%c ", lefts[i]);
    sets[i]->prettyPrint();
    printf("\n");
  }
}
