#include "FirstSets.h"

//general constructor
FirstSets::FirstSets(Production *prods[], int productionCount) {
  size = 0;
  this->prods = prods;
  this->productionCount = productionCount;
  this->lefts = new LexemSet();
}

//generate all first sets
void FirstSets::generate() {
  for(int i = 0; i < productionCount; i++) {
    firstForProd(prods[i]->left);
  }
}

//recursive function that generates a single first set by generating others
LexemSet* FirstSets::firstForProd(Lexem *l) {

  //check if the first set already exists and return
  int idx = lefts->has(l);
  if(idx >= 0) {
    return sets[idx];
  }

  //create a new first set
  LexemSet *firstSet = new LexemSet();

  //check every production
  for(int i = 0; i < productionCount; i++) {

    //if the production has the left lexem l
    if(prods[i]->left->compare(l) == 0) {
      Lexem *firstRight = prods[i]->right->items[0];

      //if the first lexem is a production (capital letter)
      if(firstRight != NULL && firstRight->l[0] >= 'A' && firstRight->l[0] <= 'Z') {

        //continue if the next production has the same left as the original
        if(firstRight->compare(l) == 0) {
          continue;
        }

        //otherwise get the set of the production
        LexemSet *newFirsts = firstForProd(firstRight);
        for(int i = 0; i < newFirsts->size; i++) {
          if(firstSet->has(newFirsts->items[i]) == -1) {
            firstSet->add(newFirsts->items[i]);
          }
        }
      }

      //if the first lexem is not a prod, add it to the set
      else {

        //if epsilon, then modify the char
        if(prods[i]->right->size == 0) {
          firstRight = new Lexem(0, (char*)"epsilon");
        }

        if(firstSet->has(firstRight) == -1) {
          firstSet->add(firstRight);
        }
      }
    }
  }

  //add the set to the list and return
  lefts->add(l);
  sets[size] = firstSet;
  size++;
  return firstSet;
}

//print the first sets nicely
void FirstSets::prettyPrint() {
  printf("First sets:\n");
  for(int i = 0; i < size; i++) {
    lefts->items[i]->prettyPrint();
    printf(" ");
    sets[i]->prettyPrint();
    printf("\n");
  }
}
