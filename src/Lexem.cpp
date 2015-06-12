#include "Lexem.h"

Lexem::Lexem(int t, char *l) {
  this->t = t;
  this->l = (char*)malloc(MAX_LEXEM_SIZE*sizeof(char));
  strcpy(this->l, l);
}

int Lexem::compare(Lexem *l) {
  return strcmp(this->l, l->l);
}

Lexem* Lexem::duplicate() {
  Lexem *newLexem = new Lexem(this->t, this->l);
  return newLexem;
}

void Lexem::prettyPrint() {
  printf("%s", l);
}
