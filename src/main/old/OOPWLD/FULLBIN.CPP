// FULLBIN.CPP
#include "fullbin.hpp"

void tree::add(char * dat) {
  if(*data == 0) { dup(dat); return; }  // use current node if empty
  int compare = strcmp(dat, data);
  if( compare > 0) { // dat > data -- right tree
    if(right) right->add(dat);  // if subtree is there, recursive call
    else right = new tree(dat); // if not, make a new subtree
  } else if(compare < 0) {  // put it in left tree
    if(left) left->add(dat); // if subtree is there, recursive call
    else left = new tree(dat);  // if not, make a new subtree
  }
  // do nothing if it's already in the tree (compare == 0)
}

tree * tree::find(char * dat) {
  int cmp = strcmp(dat,data);
  if(cmp == 0) return this;  // return this subtree if dat == data
  if(cmp > 0) { // search right subtree if dat > data
    if(right) return right->find(dat);
    else return NULL;
  }
  if(cmp < 0) { // search left subtree if dat < data
    if(left) return left->find(dat);
    else return NULL;
  }
}

void rotate_left(tree ** tp) {
//  tree * t = *tp;
  if(!(*tp)->right) return;  // no place to rotate from
  tree * tmp = (*tp)->right->left;
  (*tp)->right->left = (*tp);
  *tp = (*tp)->right;  // change the external pointer value
  (*tp)->left->right = tmp;
}

void rotate_right(tree ** tp) {
//  tree * t = *tp;
  if(!(*tp)->left) return;  // no place to rotate from
  tree * tmp = (*tp)->left->right;
  (*tp)->left->right = (*tp);
  *tp = (*tp)->left;  // change the external pointer value
  (*tp)->right->left = tmp;
}

