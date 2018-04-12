// SHAPE.CPP : methods for generic shape class
#include "shape.hpp"
#include <conio.h>

const size_step = 5;  // step value for increasing or decreasing size

shape::shape(int xc, int yc, int xscale, int yscale,
             COLORS fc, fill_patterns fp, COLORS lc)
    : xcoord(xc), ycoord(yc),
      x_proportion(xscale), y_proportion(yscale),
      line_color(lc),  fill_color(fc), fill_pattern(fp)
  {}

void shape::draw() {
  // call this, then your specific drawing function
  setcolor(line_color);
  setfillstyle(fill_pattern, fill_color);
}

void shape::erase() {
  // call this, then your specific drawing function
  setcolor(BLACK);
  setfillstyle(SOLID_FILL, BLACK);
}

void shape::moverelative(int x, int y) {
  erase();
  xcoord += x;
  ycoord += y;
}

void shape::edit() {  // change characteristics
  while(1) {
    bgi_graphics::bottomprompt(
    "p: pattern, c: color, s: size, q to quit");
    switch(getch()) {
      case 'p' : fillpattern((fill_patterns)++fill_pattern);
                 break;
      case 'c' : fillcolor((COLORS)++fill_color);
                 break;
      case 's' : bgi_graphics::bottomprompt(
                 "X for larger xval, x for smaller,"
                 "Y for larger yval, y for smaller, q to quit");
                 for(int rsp = getch(); rsp != 'q'; rsp = getch()) {
                   switch(rsp) {
                     case 'X': x_proportion += size_step; break;
                     case 'Y': y_proportion += size_step; break;
                     case 'x': x_proportion -= size_step; break;
                     case 'y': y_proportion -= size_step; break;
                   }
                   if(x_proportion <= 0) x_proportion = 1;
                   if(y_proportion <= 0) y_proportion = 1;
                   // virtual function calls:
                   erase();
                   setsize();
                   draw();
                 }
                 break;
      case 'q' : return;
      default  : break;
    }
  }
}

void shape::fillcolor(COLORS fc) {
  fill_color = fc;
  if(fill_color > bgi_graphics::maxcolor)
    fill_color = BLACK;  // wrap around "corner"
  draw();
}

void shape::fillpattern(fill_patterns fp) {
  fill_pattern = fp;
  if(fill_pattern >= USER_FILL)
    fill_pattern = EMPTY_FILL;  // wrap around "corner"
  draw();
}
