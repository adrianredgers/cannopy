// DIRECT2.HPP: Simple inheritance
#include "direct.hpp"  // the original class

class dirfile2 : public dirfile {  // <-- inheritance!
  static int printcount;  // one variable for all objects
  // statics are automatically initialized to zero; you can
  // also explicitly initialize them.
public:
  void print2() { // re-define the function
    // start a new line every 4th call:
    if(printcount++ % 4 == 0) putchar('\n');
    // use scope-resolution operator to call the base-class
    // function (defaults to derived-class function):
    dirfile::print2();
  }
  // a static member function:
  static void reset_printcount() { printcount = 0; }
};
