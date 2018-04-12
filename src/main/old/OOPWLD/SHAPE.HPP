// SHAPE.HPP : abstract base class for all shapes
#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_
#include "bgi.hpp"

class shape {
protected:
  COLORS line_color, fill_color;
  fill_patterns fill_pattern;
  int xcoord, ycoord;  // "center" coordinates
  int x_proportion, y_proportion;  // determines size of shape
public:
  shape(int xc, int yc, int xscale, int yscale,
        COLORS fc = BLACK, fill_patterns fp = EMPTY_FILL,
        COLORS lc = bgi_graphics::maxcolor);
  virtual ~shape() {}  // virtual destructor
  virtual void draw();
  virtual void erase();
  virtual void moverelative(int x, int y);
  void setxsize(int xs) { x_proportion = xs; }
  void setysize(int ys) { y_proportion = ys; }
  virtual void setsize() {}; // change to a new size
  virtual void edit();  // modify a shape according to user wishes
  virtual void fillcolor(COLORS fc = BLACK);
  virtual void fillpattern(fill_patterns fp = EMPTY_FILL);
};

#endif //  _SHAPE_HPP_
