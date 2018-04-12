// CIRCBOX.HPP : new type of shape made from Circle and Square
#ifndef _CIRCBOX_HPP_
#define _CIRCBOX_HPP_
#include "polygon.hpp"
#include "ellipse.hpp"

// member objects:
class circlebox : public shape {
  Circle C;
  Square S;
public:
  circlebox(int xc, int yc, int size = 10) :
    shape(xc, yc, size, size),
    C(xc, yc, size), S(xc, yc, size + 20) {}
  void setsize() {
    C.setxsize(x_proportion);
    C.setsize();
    S.setxsize(x_proportion + 20);
    S.setsize();
  }
  void draw() {
    S.draw();
    C.draw();
  }
  void erase() {
    S.erase();
    C.erase();
  }
  void moverelative(int x, int y) {
    shape::moverelative(x,y);
    S.moverelative(x,y);
    C.moverelative(x,y);
  }
  void fillcolor(COLORS fc = BLACK) {
    shape::fillcolor(fc);
    S.fillcolor(fc);
    C.fillcolor(fc);
  }
  void fillpattern(fill_patterns fp = EMPTY_FILL) {
    shape::fillpattern(fp);
    S.fillpattern(fp);
    C.fillpattern(fp);
  }
};

#endif // _CIRCBOX_HPP_

