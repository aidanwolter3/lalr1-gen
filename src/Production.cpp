#include "Production.h"

//general constructor
Production::Production(int id, char left, char *right) {
  this->id = id;
  this->left = left;
  this->right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
  strcpy(this->right, right);
  this->mark = 0;
  this->followSet = new Set();
  this->completed = false;
}
Production::Production(int id, char left) {
  this->id = id;
  this->left = left;
  this->right = (char*)malloc(MAX_PROD_STR_LEN*sizeof(char));
  this->mark = 0;
  this->followSet = new Set();
  this->completed = false;
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
  char *markedStr = strdup(this->right);
  strcpy(&markedStr[mark+1], &markedStr[mark]);
  markedStr[mark] = '.';
  printf("%c -> %s  ", left, markedStr);
  followSet->prettyPrint();
  printf("\n");
}

//check for equality in productions
bool Production::equals(Production *prod) {
  return (prod->left == this->left) &&
         (prod->mark == this->mark) &&
         (strcmp(prod->right, this->right) == 0) &&
         (prod->followSet->equals(this->followSet) == true);
}
