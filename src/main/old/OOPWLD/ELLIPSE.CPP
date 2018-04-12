// ELLIPSE.CPP : methods for ellipses and circles
#include "ellipse.hpp"

Ellipse::Ellipse(int xc, int yc, int xrad, int yrad,
        COLORS fc, fill_patterns fp,  COLORS lc)
   : shape(xc, yc, xrad, yrad, fc, fp, lc),
     xradius(xrad), yradius(yrad) {}

void Ellipse::draw() {
  shape::draw();
  fillellipse(xcoord, ycoord, xradius, yradius);
}

void Ellipse::erase() {
 shape::erase();
 fillellipse(xcoord, ycoord, xradius, yradius);
}

// Set the size based on the proportions:
void Ellipse::setsize() {
  xradius = x_proportion;
  yradius = y_proportion;
}

// Notice a circle's size only depends on x_proportion:
void Circle::setsize() {
  xradius = yradius = x_proportion;
}
