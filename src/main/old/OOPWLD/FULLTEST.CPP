#include "fullbin.hpp"  // full-functioned binary tree
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

char * test[] = {
  "l","e","s","c","h","o","w","b","d",
  "g","j","m","q","u","y","a","f",
  "i","k","n","p","r","t","v","x","z"
};

main() {
  tree * T = new tree;
  for(int i = 0; i < sizeof(test)/sizeof(test[0]); i++)
    T->add(test[i]);
  while(1) {
    {
      clrscr();
      screenmap screen(T->depth() + 1,80);
      T->place(screen, 0, 40, 40);
      screen.display();
      puts("\ninorder traversal:");
      T->inorder_traverse();
    }
    printf("\nbalance = %d\n", (int)T->heavy());
    switch(printf(
        "l:left rotate, r:right rotate, "
        "b:balance, a:rebalance, s: start over,"
        "q:quit "), tolower(getch())) {
      case 'l' : rotate_left(&T); break;
      case 'r' : rotate_right(&T); break;
      case 'b' : balance(&T); break;
      case 'a' : rebalance(&T); break;
      case 's' : delete T;  // start over
                 T = new tree;
                 for(int i = 0; 
                     i < sizeof(test)/sizeof(test[0]); 
                     i++)
                   T->add(test[i]);
                 break;
      case 'q' : exit(0);
    }
  }
}
