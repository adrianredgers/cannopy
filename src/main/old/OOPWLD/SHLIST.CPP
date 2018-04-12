// SHLIST.CPP : Methods for list to hold shapes
#include "shlist.hpp"

void shape_list::drawlist() {
  shape_holder * s = cursor;  // start at the current shape
  do {
    if(s) s = s->next; // find the next valid element
    if(!s && cursor) s = head; // NULL == end of list; go to head
    if(!s) return;  // still NULL -- must be empty list
    s->shp->draw();
  } while(s != cursor);  // draw cursor shape last, so it's on top
}

void shape_list::remove() {  // delete current element
  if(!cursor) return;  // nothing to remove (or at end of list)
  shape_holder * cur, * drag;
  cur = drag = head;
  while(cur != cursor) {
    drag = cur;
    cur = cur->next;
  }
  // special case -- shape at the head of the list:
  if (cur == head && head) head = head->next;
  next();  // move cursor to next position
  delete cur;
  drag->next = cursor;  // thread past deleted node
}

shape_list::~shape_list() {  // remove elements until the list is empty
  cursor = head;  // move to top of list
  while(head) { // while there are elements left at the top
    cursor = head->next; // remember the next position
    delete head; // delete the top
    head = cursor; // move the head down to the new top
  }
}
