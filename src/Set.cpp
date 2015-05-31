#include "Set.h"

//general constructor
Set::Set() {
  items = (char*)malloc(MAX_FOLLOW_SET_LEN*sizeof(char));
  size = 0;
}

//sort the set for easy comparison
void Set::sort() {
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

//add a new follow to the set. do not allow duplicates. sort the set afterwards
void Set::add(char c) {

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

//add all the items in one set to this one
void Set::add(Set *s) {
  for(int i = 0; i < s->size; i++) {
    add(s->items[i]);
  }
}

//remove a specific item
int Set::remove(char c) {
  for(int i = 0; i < size; i++) {
    if(items[i] == c) {
      strcpy(&items[i], &items[i+1]);
      size--;
      return 1;
    }
  }
  return 0;
}

//check if the item exists in the set
bool Set::has(char c) {
  for(int i = 0; i < size; i++) {
    if(items[i] == c) {
      return true;
    }
  }
  return false;
}

//check for equality between sets
bool Set::equals(Set *f) {
  return (strcmp(f->items, items) == 0);
}

//print the follow set
void Set::prettyPrint() {
  printf("{%s}", items);
}

//set setter. copies string instead of reusing
void Set::set(char *items) {
  strcpy(this->items, items);
  size = strlen(items);
}

//duplicate the follow set
Set* Set::duplicate() {
  Set *newSet = new Set();
  newSet->set(items);
  return newSet;
}
