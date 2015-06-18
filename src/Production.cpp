#include "Production.h"

//general constructor
Production::Production(int id, Lexem *left, LexemSet *right) {
  this->completed = false;
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
  if(right->size == 0 || right->size == mark) {
    printf(" .");
  }
  printf("  ");
  followSet->prettyPrint();
}

//check for equality in productions
bool Production::equals(Production *prod) {
  return (left->compare(this->left) == 0) &&
         (prod->mark == this->mark) &&
         ((prod->right->exactEquals(this->right)) == true) &&
         (prod->followSet->equals(this->followSet) == true);
}

//add a follow to the follow set and ensure that it is unique
void Production::addFollow(Lexem *l) {
  bool match = false;
  for(int i = 0; i < followSet->size; i++) {
    if(followSet->items[i]->compare(l) == 0) {
      match = true;
    }
  }
  if(match == false) {
    followSet->add(l);
  }
}
void Production::addFollow(LexemSet *s) {
  for(int i = 0; i < s->size; i++) {
    bool match = false;
    for(int j = 0; j < followSet->size; j++) {
      if(followSet->items[j]->compare(s->items[i]) == 0) {
        match = true;
      }
    }
    if(match == false) {
      followSet->add(s->items[i]);
    }
  }
}
