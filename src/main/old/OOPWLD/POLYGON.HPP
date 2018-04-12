// POLYGON.HPP: type of shape for squares, rectangles,
// triangles, etc.
#ifndef _POLYGON_HPP_
#define _POLYGON_HPP_
#include "shape.hpp"
#include <string.h>

class Polygon : public shape {
protected:
  int * relative_vertices;  // array of points determining the corners
  // note relative_vertices[0] and [1] are the "anchor coordinates"
  void translate();  // calculate the actual vertices
private:
  int * actual_vertices; // the actual coordinates of the corners
  int vcount;  // number of vertices
public:
  Polygon(int vnumber, int * vertices, int xc, int yc,
          int xsize = 10, int ysize = 10,
          COLORS fc = WHITE, fill_patterns fp = SOLID_FILL,
          COLORS lc = WHITE);
  ~Polygon();
  void draw();
  void erase();
  void moverelative(int x, int y);
};

class Rectangle : public Polygon {
protected:
  static int corners[8];
public:
  Rectangle(int xc, int yc, int xsize = 10, int ysize = 20);
  void setsize();
};

class Square : public Rectangle {
public:
  Square(int xc, int yc, int size = 10);
  void setsize();
};

class Triangle : public Polygon {
  static int corners[6];
public:
  Triangle(int xc, int yc, int size = 10);
  void setsize();
};

#endif // _POLYGON_HPP_
