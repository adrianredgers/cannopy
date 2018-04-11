// STARS.CPP: demonstration of new and delete
#include <conio.h>
#include <stdlib.h>
const scrwidth = 79;
const scrheight = 24;

class star {  // a single star
  int x, y;  // positions on the screen
public:
  star(int xpos, int ypos) : x(xpos), y(ypos) {
    gotoxy(x,y);
    cputs("*");
  }
  ~star() {
    gotoxy(x,y);
    cputs(" ");  // blank out the space
  }
};

class starbuffer {
  enum { size = 128 };  // untyped enum acts as local const
  star* ringbuffer[size];  // place to put pointers to stars
  int index;
public:
  starbuffer() : index(0) {
    for(int i = 0; i < size; i++)
      ringbuffer[i] = (star*)0;
  }
  void add(star * s) { ringbuffer[index] = s; }
  star * next() { return ringbuffer[ index = (++index % size) ]; }
};

main() {
  starbuffer stars;
  while(!kbhit()) {
    delete stars.next();  // deleting 0 does nothing
    stars.add(new star(rand() % scrwidth, rand() % scrheight));
  }
}
