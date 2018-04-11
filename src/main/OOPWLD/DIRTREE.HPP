// DIRTREE.HPP: very simple binary tree to hold directory entries.
// An inorder traversal of this tree produces a sorted list
#include "direct2.hpp"

// Add two "overloaded operators" to our directory class:
class dirfile3 : public dirfile2 {  // more inheritance
public:
  dirfile3() {} // what happens here? (base constructor called)
  dirfile3(ffblk * file_info) { // overloaded function name
    // at this point, the base class constructor has been called:
    setfile(file_info);
  }
  // reference automatically takes the address:
  int operator>(dirfile3 & d) { // overload the comparison operators
    // note we access protected members here:
    return strcmp(filename, d.filename) > 0;
  }
  int operator<(dirfile3 & d) {
    return strcmp(filename, d.filename) < 0;
  }
};

// a tiny "collection" class:
class dlist {
  dirfile3 ** dl; // points to an array of pointers
  int size, index;
  static dirfile3 df3;  // for empty places in the list
public:
  dlist(int sz) : index(0) { 
    dl = new dirfile3*[size = sz]; // space for array
    for(int i = 0; i < size; i++)
      dl[i] = &df3;  // set all elements to empty object
  }
  ~dlist() { delete dl; }  // clean up free store
  void add(dirfile3 * d) {  // put things in
    dl[index] = d;
    // don't pass the end:
    index = (index + 1 < size ? index + 1 : index);
  }
  // Operator overloading:
  dirfile3 * operator[](int i) {
    // Simple but robust error handling:
    if(i < 0) return dl[0];
    if(i >= size) return dl[size - 1];
    return dl[i];
  }
};  

// Binary tree.  A dirtree is a node AND a tree, simultaneously:
class dirtree {
  dirfile3 * data;
  dirtree * left, * right;
public:
  dirtree(dirfile3 * dat) : left(NULL), right(NULL), data(dat) {}
  ~dirtree() {}  // do something intelligent here...
  void add(dirfile3 * dat) { // insert a new node in the tree
    if( *dat > *data) { // put it in the right tree (note operator call)
      if(right) right->add(dat);  // if subtree is there, recursive call
      else right = new dirtree(dat); // if not, make a new subtree
    } else if(*dat < *data) {  // put it in left tree
      if(left) left->add(dat); // if subtree is there, recursive call
      else left = new dirtree(dat);  // if not, make a new subtree
    }
    // do nothing if it's already in the tree (dat == data)
  }
  // inorder traversal puts the sorted list sequentially into a dlist:
  void distribute(dlist & dl) {
    if(left) left->distribute(dl); // recursive call
    dl.add(data);  // in-order traversal == sorted
    if(right) right->distribute(dl); // recursive call
  }
};
