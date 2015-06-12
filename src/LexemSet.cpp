#include "LexemSet.h"

//general constructor
LexemSet::LexemSet() {
  items = (Lexem**)malloc(MAX_LEXEM_SET_LEN*sizeof(Lexem*));
  size = 0;
}

//add a new follow to the set. do not allow duplicates. sort the set afterwards
void LexemSet::add(Lexem *l) {

  //check where the lexem fits in the sorted order
  int i;
  int index = 0;
  for(i = 0; i < size; i++) {
    int diff = items[i]->compare(l);
    if(diff > 0) {
      index++;
    }
  }

  //add the lexem
  items[size] = l->duplicate();
  size++;

  //add the actual index to the sorted order
  for(int i = index; i < size; i++) {
    sortedOrder[i+1] = sortedOrder[i];
  }
  sortedOrder[i] = size-1;
}

//add all the items in one set to this one
void LexemSet::add(LexemSet *s) {
  for(int i = 0; i < s->size; i++) {
    add(s->items[i]);
  }
}

//retrieve a lexem from the string
Lexem* LexemSet::get(char *str) {
  for(int i = 0; i < size; i++) {
    if(strcmp(items[i]->l, str) == 0) {
      return items[i];
    }
  }
  return NULL;
}

//remove a specific item
int LexemSet::remove(Lexem *l) {
  for(int i = 0; i < size; i++) {
    if(items[i]->compare(l) == 0) {
      for(int j = i; j < size-1; j++) {
        items[j] = items[j+1];
      }
      size--;
      return 1;
    }
  }
  return 0;
}

//check if the item exists in the set
int LexemSet::has(Lexem *l) {
  for(int i = 0; i < size; i++) {
    if(items[i]->compare(l) == 0) {
      return i;
    }
  }
  return -1;
}

//check for equality between sets
bool LexemSet::equals(LexemSet *f) {
  bool same = true;
  for(int i = 0; i < size; i++) {
    if(items[sortedOrder[i]]->compare(f->items[f->sortedOrder[i]]) != 0) {
      same = false;
      break;
    }
  }
  return same;
}

//print the follow set
void LexemSet::prettyPrint() {
  printf("{");
  for(int i = 0; i < size; i++) {
    printf("%s", items[i]->l);
    if(i < size-1) {
      printf(", ");
    }
  }
  printf("}");
}

//duplicate the follow set
LexemSet* LexemSet::duplicate() {
  LexemSet *newSet = new LexemSet();
  for(int i = 0; i < size; i++) {
    newSet->add(items[i]);
  }
  return newSet;
}
