// ELLIPSE.HPP : A type of shape for ellipses and circles
#ifndef _ELLIPSE_HPP_
#define _ELLIPSE_HPP_
#include "shape.hpp"

class Ellipse : public shape {
protected:
  int xradius, yradius; // Ellipse radii from center
public:
  Ellipse(int xc, int yc, int xrad = 10, int yrad = 10,
          COLORS fc = BLACK, fill_patterns fp = EMPTY_FILL,
          COLORS lc = bgi_graphics::maxcolor);
  ~Ellipse() { erase(); }
  void draw();
  void erase();
  void setsize();
};

// A Circle is an Ellipse with only one radius
class Circle : public Ellipse {
public:
  Circle(int xc, int yc, int radius = 10) :
    Ellipse(xc, yc, radius, radius) {}
  void setsize();
};

#endif // _ELLIPSE_HPP_