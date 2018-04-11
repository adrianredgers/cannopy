// POLYGON.CPP:  Methods for class polygon
#include "polygon.hpp"
#include <string.h>

Polygon::Polygon(int vnumber, int * vertices, int xc, int yc,
                 int xsize, int ysize,
                 COLORS fc, fill_patterns fp, COLORS lc)
      : shape(xc, yc, xsize, ysize, fc, fp, lc), vcount(vnumber) {
  if(vcount <= 0) vcount = 1;  // must have at least one vertice
  relative_vertices = new int[vcount * 2];
  actual_vertices = new int[vcount * 2];
  memcpy(relative_vertices, vertices, vcount * 2 * sizeof(int));
  translate();  // calculate actual_vertices
}

void Polygon::translate() {
  memcpy(actual_vertices, relative_vertices, vcount * 2 * sizeof(int));
  for(int i = 0; i < vcount * 2; i += 2) {
    actual_vertices[i] += xcoord;
    actual_vertices[i+1] += ycoord;
  }
}

Polygon::~Polygon() {
  erase();
  delete relative_vertices;
  delete actual_vertices;
}

void Polygon::draw() {
  shape::draw();
  fillpoly(vcount, actual_vertices);
}

void Polygon::erase() {
  shape::erase();
  fillpoly(vcount, actual_vertices);
  drawpoly(vcount, actual_vertices);  // erase "trailing" line...
}

void Polygon::moverelative(int x, int y) {
  erase();
  xcoord += x;
  ycoord += y;
  translate();
  draw();
}

Rectangle::Rectangle(int xc, int yc, int xsize, int ysize)
  : Polygon(4, corners, xc, yc, xsize, ysize) {
    setsize();  // virtual mechanism not active in constructors
}

void Rectangle::setsize() {
  relative_vertices[0] = 0;
  relative_vertices[1] = 0;
  relative_vertices[2] = x_proportion;
  relative_vertices[3] = 0;
  relative_vertices[4] = x_proportion;
  relative_vertices[5] = y_proportion;
  relative_vertices[6] = 0;
  relative_vertices[7] = y_proportion;
  translate();
}

Square::Square(int xc, int yc, int size) :
  Rectangle(xc, yc, size, size) {
    setsize();  // virtual mechanism not active in constructors
}

void Square::setsize() {
  relative_vertices[0] = 0;
  relative_vertices[1] = 0;
  relative_vertices[2] = x_proportion;
  relative_vertices[3] = 0;
  relative_vertices[4] = x_proportion;
  relative_vertices[5] = x_proportion;
  relative_vertices[6] = 0;
  relative_vertices[7] = x_proportion;
  translate();
}

Triangle::Triangle(int xc, int yc, int size) :
  Polygon(3, corners, xc, yc, size) {
    setsize();  // virtual mechanism not active in constructors
}

void Triangle::setsize() {
  relative_vertices[0] = 0;
  relative_vertices[1] = 0;
  relative_vertices[2] = x_proportion;
  relative_vertices[3] = 0;
  relative_vertices[4] = x_proportion/2;
  relative_vertices[5] = y_proportion;
  translate();
}
