#include "CharSet.h"

//general constructor
CharSet::CharSet() {
  items = (char*)malloc(MAX_FOLLOW_SET_LEN*sizeof(char));
  size = 0;
}

//sort the CharSet for easy comparison
void CharSet::sort() {
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

//add a new follow to the CharSet. do not allow duplicates. sort the CharSet afterwards
void CharSet::add(char c) {

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

//add all the items in one CharSet to this one
void CharSet::add(CharSet *s) {
  for(int i = 0; i < s->size; i++) {
    add(s->items[i]);
  }
}

//remove a specific item
int CharSet::remove(char c) {
  for(int i = 0; i < size; i++) {
    if(items[i] == c) {
      strcpy(&items[i], &items[i+1]);
      size--;
      return 1;
    }
  }
  return 0;
}

//check if the item exists in the CharSet
bool CharSet::has(char c) {
  for(int i = 0; i < size; i++) {
    if(items[i] == c) {
      return true;
    }
  }
  return false;
}

//check for equality between CharSets
bool CharSet::equals(CharSet *f) {
  return (strcmp(f->items, items) == 0);
}

//print the follow CharSet
void CharSet::prettyPrint() {
  printf("{%s}", items);
}

//CharSet setter. copies string instead of reusing
void CharSet::set(char *items) {
  strcpy(this->items, items);
  size = strlen(items);
}

//duplicate the follow CharSet
CharSet* CharSet::duplicate() {
  CharSet *newCharSet = new CharSet();
  newCharSet->set(items);
  return newCharSet;
}
