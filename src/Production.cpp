#include "Production.h"

//general constructor
Production::Production(int id, Lexem *left, LexemSet *right) {
  this->id = id;
  this->left = left->duplicate();
  this->right = right->duplicate();
  this->mark = 0;
  this->followSet = new LexemSet();
}

//duplicate the production
Production* Production::duplicate() {
  Production *prod = new Production(id, left, right);
  prod->mark = mark;
  prod->followSet = followSet->duplicate();
  return prod;
}

//print the production
void Production::prettyPrint() {
  printf("%s ->", left->l);
  for(int i = 0; i < right->size; i++) {
    if(mark == i) {
      printf(" .");
    }
    printf(" %s", right->items[i]->l);
  }
  if(right->size == 0) {
    printf(" .");
  }
}

//check for equality in productions
bool Production::equals(Production *prod) {
  return (left->compare(this->left) == 0) &&
         (prod->mark == this->mark) &&
         ((prod->right->equals(this->right)) == true) &&
         (prod->followSet->equals(this->followSet) == true);
}
