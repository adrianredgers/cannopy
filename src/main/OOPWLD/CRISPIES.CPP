// CRISPIES.CPP : virtual functions made simple
#include <stdio.h>
struct rice {    // Base class
  virtual void talk() {}  // common interface to all types of rice
};
struct snap : rice {
  void talk() { puts("SNAP!"); }  // different implementations for each type
};
struct crackle : rice {
  void talk() { puts("CRACKLE!"); }
};
struct pop : rice {
  void talk() { puts("POP!"); }
};

main() {
  rice * bowl[] = { new snap, new crackle, new pop };
  for(int i = 0; i < sizeof(bowl)/sizeof(bowl[0]); i++)
    bowl[i]->talk();
}