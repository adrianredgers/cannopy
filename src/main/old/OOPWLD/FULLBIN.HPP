// FULLBIN.HPP: Full-featured binary tree.  Note use of recursive 
// calls; this isn't particularly risky because balanced binary 
// trees tend to be relatively shallow.
#include "smap.hpp"
#include <string.h>
#include <stdio.h>

class tree {
  char * data;
  tree * left, * right;
  void dup(char * dat) {  // use borland's dup() instead, for smallness
    delete data;
    data = new char[strlen(dat) + 1];
    strcpy(data,dat);
  }
  // indicators of the balance of the tree:
  enum treebalance { balanced = 0, heavy_left, heavy_right };
public:
  // When constructor is called with no arguments, makes an empty node:
  tree(char * dat = "") : left(NULL), right(NULL), data(NULL) { dup(dat); }
  ~tree() { delete data; }
  void add(char * dat);
  tree * find(char * dat);
  void preorder_traverse() {
    printf(" %s ", data);
    if(left) left->preorder_traverse();
    if(right) right->preorder_traverse();
  }
  void inorder_traverse() {
    if(left) left->inorder_traverse();
    printf(" %s ", data);
    if(right) right->inorder_traverse();
  }
  void postorder_traverse() {
    if(left) left->postorder_traverse();
    if(right) right->postorder_traverse();
    printf(" %s ", data);
  }
  int depth(int curr_depth = 0) {  // measure depth of tree
    int dl, dr; dl = dr = curr_depth;
    if(left) dl = left->depth(dl + 1);  // plumb the left depth
    if(right) dr = right->depth(dr + 1); // plumb the right depth
    return dl > dr ? dl : dr;
  }
  friend void rotate_left(tree ** tp);
  friend void rotate_right(tree ** tp);
  treebalance heavy() { // tells you how the tree is balanced
    if(!left && !right) return balanced;
    if(!left) return (right->depth() > 1) ? heavy_right : balanced;
    if(!right) return (left->depth() > 1) ? heavy_left : balanced;
    if(left->depth() - 1 > right->depth())
      return heavy_left;
    if(right->depth() - 1 > left->depth())
      return heavy_right;
    return balanced;
  }
  friend void balance(tree ** tp) {  // balance the tree
    while((*tp)->heavy() == tree::heavy_right)
      rotate_left(tp);
    while((*tp)->heavy() == tree::heavy_left)
      rotate_right(tp);
	}
  friend void rebalance(tree **tp) {
    if((*tp)->left) rebalance(&((*tp)->left));
    if((*tp)->right) rebalance(&((*tp)->right));
    while((*tp)->heavy() == tree::heavy_right)
      rotate_left(tp);
    while((*tp)->heavy() == tree::heavy_left)
      rotate_right(tp);
	}
  void place(screenmap & screen, int depth, int location, int bisection) {
    screen(depth,location) = *data;  // only put one character in as a test
    if(left) 
      left->place(screen, depth+1, location - bisection/2, bisection/2);
    if(right) 
      right->place(screen, depth+1, location + bisection/2, bisection/2);
  }
};
