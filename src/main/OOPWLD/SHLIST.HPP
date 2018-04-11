// SHLIST.HPP : Extremely simple linked list to hold shapes
#ifndef _SHLIST_HPP_
#define _SHLIST_HPP_
#include "shape.hpp"
#include <stdlib.h>

struct shape_holder {
  shape * shp;  // pointer to the actual shape
  shape_holder * next;  // link to next shape_holder in list.
  shape_holder(shape * sh, shape_holder * nx)
    : shp(sh), next(nx) {}  // very simple constructor
  virtual ~shape_holder() { delete shp; }
};

class shape_list {  // The container class
  shape_holder *head, *cursor;  // head always points to top of list
public:
  // initialize an empty list:
  shape_list() : head(NULL), cursor(NULL) {}
  // Paste a new shape_holder in at the top:
  shape * add(shape * sh) {
    head = cursor = new shape_holder(sh, head); // (This is tricky)
    return sh;
  }
  // top() and next() return the current shape or NULL, for the end:
  shape * top() {
    cursor = head; // go to top of the list
    return cursor ? cursor->shp : NULL; // return shp if not empty
  }
  shape * next() {  // Only move forward if cursor isn't NULL:
    if(cursor) cursor = cursor->next;
    return cursor ? cursor->shp : NULL; // return shp if not empty
  }
  void drawlist();
  void remove();  // remove current element
  ~shape_list();
};
#endif // _SHLIST_HPP_
